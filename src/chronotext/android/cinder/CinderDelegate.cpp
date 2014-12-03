/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/android/cinder/JNI.h"
#include "chronotext/FileSystem.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/utils/accel/AccelEvent.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chronotext
{
    bool CinderDelegate::LOG_VERBOSE = false;
    
    CinderDelegate::CinderDelegate()
    :
    sketch(nullptr)
    {}
    
    CinderSketch* CinderDelegate::getSketch()
    {
        return sketch;
    }
    
    void CinderDelegate::setSketch(CinderSketch *newSketch)
    {
        if (sketch)
        {
            destroySketch(sketch);
            sketchDestroyed(sketch);
            sketch = nullptr;
        }
        
        if (newSketch)
        {
            sketch = newSketch;
            sketch->setDelegate(this);
            sketchCreated(sketch);
        }
    }
    
    /*
     * CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
     */
    
    void CinderDelegate::prelaunch(JNIEnv *env, jobject javaContext, jobject javaListener, jobject javaDisplay, int displayWidth, int displayHeight, float displayDensity)
    {
        mJavaContext = javaContext;
        mJavaListener = javaListener;
        mJavaDisplay = javaDisplay; // TODO: SHOULD BE PART OF DisplayHelper?
        
        displayInfo = DisplayInfo::createWithDensity(displayWidth, displayHeight, displayDensity);
        
        // ---

        jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(mJavaContext), "getAssets", "()Landroid/content/res/AssetManager;");
        AAssetManager *assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(mJavaContext, getAssetsMethod));
        
        FileSystem::setAndroidAssetManager(assetManager);
        
        // ---
        
        jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(mJavaContext), "getFilesDir", "()Ljava/io/File;");
        jobject filesDirObject = env->CallObjectMethod(mJavaContext, getFilesDirMethod);
        jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
        jstring absolutePathString = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
        
        const char *internalDataPath = env->GetStringUTFChars(absolutePathString, nullptr);
        FileSystem::setAndroidInternalDataPath(internalDataPath);
        env->ReleaseStringUTFChars(absolutePathString, internalDataPath);
        
        // ---
        
        jclass environmentClass = env->FindClass("android/os/Environment");
        jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory",  "()Ljava/io/File;");
        jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
        absolutePathString = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
        
        const char *externalDataPath = env->GetStringUTFChars(absolutePathString, nullptr);
        FileSystem::setAndroidExternalDataPath(externalDataPath);
        env->ReleaseStringUTFChars(absolutePathString, externalDataPath);
        
        // ---
        
        jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(mJavaContext), "getPackageCodePath", "()Ljava/lang/String;");
        jstring packageCodePathString = (jstring)env->CallObjectMethod(mJavaContext, getPackageCodePathMethod);
        
        const char *apkPath = env->GetStringUTFChars(packageCodePathString, nullptr);
        FileSystem::setAndroidApkPath(apkPath);
        env->ReleaseStringUTFChars(packageCodePathString, apkPath);
        
        // ---
        
        /*
         * TODO: INIT CONTEXT
         */
        
        setSketch(createSketch());
    }
    
    /*
     * CALLED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
     */
    
    void CinderDelegate::setup(int width, int height)
    {
        windowInfo = WindowInfo(width, height);
        
        // ---
        
        /*
         * BOTH ARE ASSOCIATED WITH THE RENDERER'S THREAD
         */
        createSensorEventQueue();
        startIOService();
        
        sketch->Handler::setIOService(*io);
        sketch->timeline().stepTo(0);
        
        sketch->setup();
    }
    
    void CinderDelegate::shutdown()
    {
        stopIOService();
        destroySensorEventQueue();
        
        sketch->shutdown();
        setSketch(nullptr);
        
        /*
         * TODO: UN-INIT CONTEXT
         */
    }
    
    void CinderDelegate::resize(int width, int height)
    {
        windowInfo.size = Vec2i(width, height); // TODO: WE COULD FILTER SPURIOUS RESIZE EVENT LIKE IN ios/cinder/CinderDelegate.mm
        sketch->resize();
    }
    
    void CinderDelegate::event(int eventId)
    {
        switch (eventId)
        {
            case EVENT_RESUMED:
                start(CinderSketch::FLAG_APP_RESUMED);
                break;
                
            case EVENT_SHOWN:
                start(CinderSketch::FLAG_APP_SHOWN);
                break;
                
            case EVENT_PAUSED:
                stop(CinderSketch::FLAG_APP_PAUSED);
                break;
                
            case EVENT_HIDDEN:
                stop(CinderSketch::FLAG_APP_HIDDEN);
                break;
                
            case EVENT_CONTEXT_LOST:
                sketch->event(CinderSketch::EVENT_CONTEXT_LOST);
                break;
                
            case EVENT_CONTEXT_RENEWED:
                sketch->event(CinderSketch::EVENT_CONTEXT_RENEWED);
                break;
                
            case EVENT_BACKGROUND:
                sketch->event(CinderSketch::EVENT_BACKGROUND);
                break;
                
            case EVENT_FOREGROUND:
                sketch->event(CinderSketch::EVENT_FOREGROUND);
                break;
                
            case EVENT_BACK_KEY:
                sketch->event(CinderSketch::EVENT_BACK_KEY);
                break;
        }
    }
    
    void CinderDelegate::start(int flags)
    {
        mFrameCount = 0;
        
        mTimer.start();
        sketch->clock().start();
        
        sketch->start(flags);
    }
    
    void CinderDelegate::stop(int flags)
    {
        mTimer.stop();
        sketch->clock().stop();
        
        sketch->stop(flags);
    }
    
    void CinderDelegate::draw()
    {
        sketch->clock().update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME

        pollSensorEvents(); // WHERE accelerated IS INVOKED
        pollIOService(); // WHERE addTouch, updateTouch, removeTouch, ETC. ARE INVOKED
        
        /*
         * MUST BE CALLED BEFORE Sketch::update
         * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
         *
         * NOTE THAT getTime() COULD HAVE BEEN ALREADY CALLED
         * WITHIN ONE OF THE PREVIOUSLY "POLLED" FUNCTIONS
         */
        double now = sketch->clock().getTime();
        
        sketch->update();
        sketch->timeline().stepTo(now);
        mFrameCount++;

        sketch->draw();
    }
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    ostream& CinderDelegate::console()
    {
        if (!mOutputStream)
        {
            mOutputStream = make_shared<android::dostream>();
        }
        
        return *mOutputStream;
    }
    
    boost::asio::io_service& CinderDelegate::io_service() const
    {
        return *io;
    }
    
    double CinderDelegate::getElapsedSeconds() const
    {
        return mTimer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-VALUE PER FRAME
    }
    
    uint32_t CinderDelegate::getElapsedFrames() const
    {
        return mFrameCount;
    }
    
    bool CinderDelegate::isEmulated() const
    {
        return false;
    }
    
    WindowInfo CinderDelegate::getWindowInfo() const
    {
        return windowInfo;
    }
    
    DisplayInfo CinderDelegate::getDisplayInfo() const
    {
        return displayInfo;
    }
    
#pragma mark ---------------------------------------- IO SERVICE ----------------------------------------

    void CinderDelegate::startIOService()
    {
        io = make_shared<boost::asio::io_service>();
        ioWork = make_shared<boost::asio::io_service::work>(*io);
    }
    
    void CinderDelegate::stopIOService()
    {
        io->stop();
    }
    
    void CinderDelegate::pollIOService()
    {
        io->poll();
    }
    
#pragma mark ---------------------------------------- SENSOR EVENTS ----------------------------------------

    void CinderDelegate::createSensorEventQueue()
    {
        auto looper = ALooper_forThread();
        
        if (!looper)
        {
            looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        }
        
        mSensorManager = ASensorManager_getInstance();
        mAccelerometerSensor = ASensorManager_getDefaultSensor(mSensorManager, ASENSOR_TYPE_ACCELEROMETER);
        mSensorEventQueue = ASensorManager_createEventQueue(mSensorManager, looper, 3, nullptr, nullptr);
    }
    
    void CinderDelegate::destroySensorEventQueue()
    {
        ASensorManager_destroyEventQueue(mSensorManager, mSensorEventQueue);
    }
    
    void CinderDelegate::pollSensorEvents()
    {
        ASensorEvent event;
        
        while (ASensorEventQueue_getEvents(mSensorEventQueue, &event, 1) > 0)
        {
            if (event.type == ASENSOR_TYPE_ACCELEROMETER)
            {
                handleAcceleration(event);
            }
        }
    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

    /*
     * REFERENCES:
     *
     * http://android-developers.blogspot.co.il/2010/09/one-screen-turn-deserves-another.html
     * http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
     */
    static void canonicalToWorld(int displayRotation, float *canVec, ci::Vec3f &worldVec)
    {
        struct AxisSwap
        {
            int negateX;
            int negateY;
            int xSrc;
            int ySrc;
        };
        
        static const AxisSwap axisSwap[] =
        {
            { 1,  1, 0, 1 }, // ROTATION_0
            {-1,  1, 1, 0 }, // ROTATION_90
            {-1, -1, 0, 1 }, // ROTATION_180
            { 1, -1, 1, 0 }  // ROTATION_270
        };
        
        const AxisSwap &as = axisSwap[displayRotation];
        
        worldVec.x = as.negateX * canVec[as.xSrc];
        worldVec.y = as.negateY * canVec[as.ySrc];
        worldVec.z = canVec[2];
    }
    
    void CinderDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        accelFilter = AccelEvent::Filter(filterFactor);
        
        int delay = 1000000 / updateFrequency;
        int min = ASensor_getMinDelay(mAccelerometerSensor);
        
        if (delay < min)
        {
            delay = min;
        }
        
        ASensorEventQueue_enableSensor(mSensorEventQueue, mAccelerometerSensor);
        ASensorEventQueue_setEventRate(mSensorEventQueue, mAccelerometerSensor, delay);
    }
    
    void CinderDelegate::disableAccelerometer()
    {
        ASensorEventQueue_disableSensor(mSensorEventQueue, mAccelerometerSensor);
    }
    
    void CinderDelegate::handleAcceleration(ASensorEvent event)
    {
        int displayRotation = getDisplayRotation();

        Vec3f transformed;
        canonicalToWorld(displayRotation, (float*)&event.acceleration.v, transformed);
        
        /*
         * ADDITIONAL TRANSFORMATION: FOR CONSISTENCY WITH iOS
         */
        transformed *= Vec3f(-1, -1, +1) / ASENSOR_STANDARD_GRAVITY; // TODO: DOUBLE-CHECK Z AXIS
        
        sketch->accelerated(accelFilter.process(transformed));
    }
    
    /*
     * TODO: SHOULD BE PART OF DisplayHelper?
     */
    
    int CinderDelegate::getDisplayRotation()
    {
        JNIEnv *env = jvm::env();
        jmethodID getRotationMethod = env->GetMethodID(env->GetObjectClass(mJavaDisplay), "getRotation", "()I");
        return env->CallIntMethod(mJavaDisplay, getRotationMethod);
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------

    void CinderDelegate::addTouch(int index, float x, float y)
    {
        sketch->addTouch(index, x, y);
    }
    
    void CinderDelegate::updateTouch(int index, float x, float y)
    {
        sketch->updateTouch(index, x, y);
    }
    
    void CinderDelegate::removeTouch(int index, float x, float y)
    {
        sketch->removeTouch(index, x, y);
    }
    
#pragma mark ---------------------------------------- MESSAGES AND ACTIONS ----------------------------------------

    void CinderDelegate::action(int actionId)
    {
        callVoidMethodOnJavaListener("action", "(I)V", actionId);
    }
    
    /*
     * TODO: FORMAT body FOR LOG
     *
     * 1) LEADING AND TRAILING WHITE-SPACE TRIMMED
     * 2) LINE-BREAKS AND TABS REPLACED BY SPACES
     * 3) TEXT-LENGTH LIMITED
     */
    
    void CinderDelegate::receiveMessageFromSketch(int what, const string &body)
    {
        LOGI_IF(LOG_VERBOSE) << "MESSAGE SENT TO JAVA: " << what << " " << body << endl;
        callVoidMethodOnJavaListener("receiveMessageFromSketch", "(ILjava/lang/String;)V", what, jvm::env()->NewStringUTF(body.data()));
    }
    
    void CinderDelegate::sendMessageToSketch(int what, const string &body)
    {
        LOGI_IF(LOG_VERBOSE) << "MESSAGE RECEIVED FROM JAVA: " << what << " " << body << endl;
        sketch->sendMessage(Message(what, body));
    }
    
#pragma mark ---------------------------------------- JAVA LISTENER ----------------------------------------

    void CinderDelegate::callVoidMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        env->CallVoidMethodV(mJavaListener, method, args);
        va_end(args);
    }
    
    jboolean CinderDelegate::callBooleanMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jboolean ret = env->CallBooleanMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jchar CinderDelegate::callCharMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jchar ret = env->CallCharMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jint CinderDelegate::callIntMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jint ret = env->CallIntMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jlong CinderDelegate::callLongMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jlong ret = env->CallLongMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jfloat CinderDelegate::callFloatMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jfloat ret = env->CallFloatMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jdouble CinderDelegate::callDoubleMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jdouble ret = env->CallDoubleMethod(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jobject CinderDelegate::callObjectMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jvm::env();
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jobject ret = env->CallObjectMethodV(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
}
