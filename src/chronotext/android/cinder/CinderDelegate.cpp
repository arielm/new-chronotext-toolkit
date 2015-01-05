/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/Context.h"
#include "chronotext/android/cinder/JNI.h"
#include "chronotext/utils/accel/AccelEvent.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    atomic<bool> CinderDelegate::LOG_VERBOSE (false);
    atomic<bool> CinderDelegate::LOG_WARNING (true);
    
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
    
    void CinderDelegate::init(JNIEnv *env, jobject androidContext, jobject androidDisplay, int displayWidth, int displayHeight, float displayDensity)
    {
        bootInfo.androidContext = androidContext;
        bootInfo.androidDisplay = androidDisplay;
        
        bootInfo.displayInfo = DisplayInfo::createWithDensity(displayWidth, displayHeight, displayDensity);

        // ---
        
        /*
         * TODO: THE FOLLOWING COULD BE HANDLED IN FileHelper
         */

        jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(androidContext), "getAssets", "()Landroid/content/res/AssetManager;");
        AAssetManager *assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(androidContext, getAssetsMethod));
        
        FileHelper::setAndroidAssetManager(assetManager);
        
        //
        
        jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(androidContext), "getFilesDir", "()Ljava/io/File;");
        jobject filesDirObject = env->CallObjectMethod(androidContext, getFilesDirMethod);
        jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
        jstring absolutePathString = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
        
        const char *internalDataPath = env->GetStringUTFChars(absolutePathString, nullptr);
        FileHelper::setAndroidInternalDataPath(internalDataPath);
        env->ReleaseStringUTFChars(absolutePathString, internalDataPath);
        
        //
        
        jclass environmentClass = env->FindClass("android/os/Environment");
        jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory",  "()Ljava/io/File;");
        jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
        absolutePathString = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
        
        const char *externalDataPath = env->GetStringUTFChars(absolutePathString, nullptr);
        FileHelper::setAndroidExternalDataPath(externalDataPath);
        env->ReleaseStringUTFChars(absolutePathString, externalDataPath);
        
        //
        
        jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(androidContext), "getPackageCodePath", "()Ljava/lang/String;");
        jstring packageCodePathString = (jstring)env->CallObjectMethod(androidContext, getPackageCodePathMethod);
        
        const char *apkPath = env->GetStringUTFChars(packageCodePathString, nullptr);
        FileHelper::setAndroidApkPath(apkPath);
        env->ReleaseStringUTFChars(packageCodePathString, apkPath);
        
        // ---
        
        CONTEXT::init(bootInfo);
        
        setSketch(createSketch());
        sketch->init();
    }
    
    void CinderDelegate::launch(JNIEnv *env)
    {}
    
    void CinderDelegate::setup(int width, int height)
    {
        windowInfo = WindowInfo(width, height);
        
        createSensorEventQueue();
        startIOService();
        
        CONTEXT::setup(*io);
        
        sketch->timeline().stepTo(0);
        sketch->setup();
    }
    
    void CinderDelegate::shutdown()
    {
        destroySensorEventQueue();
        
        sketch->shutdown();
        setSketch(nullptr);

        /*
         * TODO:
         *
         * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * - SEE RELATED TODOS IN Context AND TaskManager
         */
        stopIOService();
        CONTEXT::shutdown();
    }
    
    void CinderDelegate::resize(int width, int height)
    {
        windowInfo.size = Vec2i(width, height); // TODO: WE COULD FILTER SPURIOUS RESIZE EVENT LIKE IN ios/cinder/CinderDelegate.mm
        sketch->resize();
    }
    
    void CinderDelegate::draw()
    {
        sketch->clock()->update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME

        pollSensorEvents(); // WHERE accelerated IS INVOKED
        pollIOService(); // WHERE addTouch, updateTouch, removeTouch, ETC. ARE INVOKED
        
        /*
         * MUST BE CALLED BEFORE Sketch::update
         * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
         *
         * NOTE THAT getTime() COULD HAVE BEEN ALREADY CALLED
         * WITHIN ONE OF THE PREVIOUSLY "POLLED" FUNCTIONS
         */
        double now = sketch->clock()->getTime();
        
        /*
         * TODO: CALL memory::Manager::update()
         */

        sketch->update();
        sketch->timeline().stepTo(now);
        frameCount++;

        sketch->draw();
    }
    
#pragma mark ---------------------------------------- LIFE-CYCLE ----------------------------------------
    
    void CinderDelegate::start(CinderSketch::Reason reason)
    {
        frameCount = 0;
        
        timer.start();
        sketch->clock()->start();
        
        sketch->start(reason);
    }
    
    void CinderDelegate::stop(CinderSketch::Reason reason)
    {
        timer.stop();
        sketch->clock()->stop();
        
        sketch->stop(reason);
    }
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------
    
    double CinderDelegate::getElapsedSeconds() const
    {
        return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-VALUE PER FRAME
    }
    
    uint32_t CinderDelegate::getElapsedFrames() const
    {
        return frameCount;
    }
    
    bool CinderDelegate::isEmulated() const
    {
        return false;
    }
    
    const WindowInfo& CinderDelegate::getWindowInfo() const
    {
        return windowInfo;
    }
    
#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------

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
        
        sensorManager = ASensorManager_getInstance();
        accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
        sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 3, nullptr, nullptr);
    }
    
    void CinderDelegate::destroySensorEventQueue()
    {
        ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
    }
    
    void CinderDelegate::pollSensorEvents()
    {
        ASensorEvent event;
        
        while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
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
        int min = ASensor_getMinDelay(accelerometerSensor);
        
        if (delay < min)
        {
            delay = min;
        }
        
        ASensorEventQueue_enableSensor(sensorEventQueue, accelerometerSensor);
        ASensorEventQueue_setEventRate(sensorEventQueue, accelerometerSensor, delay);
    }
    
    void CinderDelegate::disableAccelerometer()
    {
        ASensorEventQueue_disableSensor(sensorEventQueue, accelerometerSensor);
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
     * TODO: COULD BE HANDLED IN DisplayHelper
     */
    int CinderDelegate::getDisplayRotation()
    {
        JNIEnv *env = jni::env();
        jmethodID getRotationMethod = env->GetMethodID(env->GetObjectClass(bootInfo.androidDisplay), "getRotation", "()I");
        return env->CallIntMethod(bootInfo.androidDisplay, getRotationMethod);
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
    
#pragma mark ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------

    void CinderDelegate::event(int eventId)
    {
        switch (eventId)
        {
            case EVENT_RESUMED:
                start(CinderSketch::REASON_APP_RESUMED);
                break;
                
            case EVENT_SHOWN:
                start(CinderSketch::REASON_APP_SHOWN);
                break;
                
            case EVENT_PAUSED:
                stop(CinderSketch::REASON_APP_PAUSED);
                break;
                
            case EVENT_HIDDEN:
                stop(CinderSketch::REASON_APP_HIDDEN);
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
    
    void CinderDelegate::action(int actionId)
    {
        callVoidMethodOnListener("action", "(I)V", actionId);
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
        
        callVoidMethodOnListener("receiveMessageFromSketch", "(ILjava/lang/String;)V", what, jni::toJString(body));
    }
    
    void CinderDelegate::sendMessageToSketch(int what, const string &body)
    {
        LOGI_IF(LOG_VERBOSE) << "MESSAGE RECEIVED FROM JAVA: " << what << " " << body << endl;
        
        sketch->sendMessage(Message(what, body));
    }
    
#pragma mark ---------------------------------------- JAVA LISTENER ----------------------------------------

    /*
     * CURRENT LIMITATION: MUST BE CALLED FROM THE MAIN-THREAD OR THE RENDERER'S THREAD
     *
     * TODO:
     *
     * 1) ADD SUPPORT FOR JAVA-THREAD-ATTACHMENT IN os/Task
     * 2) ADD THREAD-LOCK
     */
    
    JsonTree CinderDelegate::jsonQuery(const char *methodName)
    {
        const string &query = jni::toString((jstring)callObjectMethodOnListener(methodName, "()Ljava/lang/String;"));
        
        if (!query.empty())
        {
            try
            {
                return JsonTree(query);
            }
            catch (exception &e)
            {
                LOGI_IF(LOG_WARNING)  << "JSON-QUERY FAILED | REASON: " << e.what() << endl;
            }
        }
        
        return JsonTree();
    }

    // ---
    
    void CinderDelegate::callVoidMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        env->CallVoidMethodV(jni::listener, method, args);
        va_end(args);
    }
    
    jboolean CinderDelegate::callBooleanMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jboolean ret = env->CallBooleanMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jchar CinderDelegate::callCharMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jchar ret = env->CallCharMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jint CinderDelegate::callIntMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jint ret = env->CallIntMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jlong CinderDelegate::callLongMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jlong ret = env->CallLongMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jfloat CinderDelegate::callFloatMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jfloat ret = env->CallFloatMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jdouble CinderDelegate::callDoubleMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jdouble ret = env->CallDoubleMethod(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
    
    jobject CinderDelegate::callObjectMethodOnListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env = jni::env();
        
        jclass cls = env->GetObjectClass(jni::listener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jobject ret = env->CallObjectMethodV(jni::listener, method, args);
        va_end(args);
        
        return ret;
    }
}
