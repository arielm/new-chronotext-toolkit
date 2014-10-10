/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/FileSystem.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/utils/accel/AccelEvent.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace std;
using namespace ci;
using namespace app;

const float GRAVITY_EARTH = 9.80665f;

namespace chronotext
{
    /*
     * CALLED ON THE RENDERER'S THREAD, BEFORE GL-CONTEXT IS CREATED
     */
    void CinderDelegate::launch(JavaVM *javaVM, jobject javaContext, jobject javaListener, jobject javaDisplay, float diagonal, float density)
    {
        mJavaVM = javaVM;
        mJavaContext = javaContext;
        mJavaListener = javaListener;
        mJavaDisplay = javaDisplay;
        
        JNIEnv *env;
        javaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        // ---
        
        displayInfo.diagonal = diagonal;
        displayInfo.density = density;
        
        SystemInfo::instance().setDisplayInfo(displayInfo);
        
        // ---
        
        jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getAssets", "()Landroid/content/res/AssetManager;");
        AAssetManager *assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(javaContext, getAssetsMethod));
        
        FileSystem::setAndroidAssetManager(assetManager);
        
        // ---
        
        jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getFilesDir", "()Ljava/io/File;");
        jobject filesDirObject = env->CallObjectMethod(javaContext, getFilesDirMethod);
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
        
        jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getPackageCodePath", "()Ljava/lang/String;");
        jstring packageCodePathString = (jstring)env->CallObjectMethod(javaContext, getPackageCodePathMethod);
        
        const char *apkPath = env->GetStringUTFChars(packageCodePathString, nullptr);
        FileSystem::setAndroidApkPath(apkPath);
        env->ReleaseStringUTFChars(packageCodePathString, apkPath);
        
        // ---
        
        ALooper *looper = ALooper_forThread();
        
        if (!looper)
        {
            looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        }
        
        mSensorManager = ASensorManager_getInstance();
        mAccelerometerSensor = ASensorManager_getDefaultSensor(mSensorManager, ASENSOR_TYPE_ACCELEROMETER);
        mSensorEventQueue = ASensorManager_createEventQueue(mSensorManager, looper, 3, nullptr, nullptr);
    }
    
    void CinderDelegate::setup(int width, int height)
    {
        windowInfo.size = Vec2i(width, height);
        
        io = make_shared<boost::asio::io_service>();
        ioWork = make_shared<boost::asio::io_service::work>(*io);
        
        sketch->setIOService(*io);
        sketch->timeline().stepTo(0);
        
        sketch->setup();
    }
    
    void CinderDelegate::shutdown()
    {
        ASensorManager_destroyEventQueue(mSensorManager, mSensorEventQueue);
        io->stop();
        
        sketch->shutdown();
        delete sketch;
    }
    
    void CinderDelegate::resize(int width, int height)
    {
        windowInfo.size = Vec2i(width, height);
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
    
    void CinderDelegate::draw()
    {
        sketch->clock().update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME

        pollSensorEvents(); // WHERE accelerated IS INVOKED
        io->poll(); // WHERE addTouch, updateTouch, removeTouch, ETC. ARE INVOKED
        
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
    
    void CinderDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        mAccelFilterFactor = filterFactor;
        
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
    
    ostream& CinderDelegate::console()
    {
        if (!mOutputStream)
        {
            mOutputStream = shared_ptr<cinder::android::dostream>(new android::dostream);
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
    
    void CinderDelegate::action(int actionId)
    {
        callVoidMethodOnJavaListener("action", "(I)V", actionId);
    }

    void CinderDelegate::receiveMessageFromSketch(int what, const string &body)
    {
#ifdef DEBUG_MESSAGES
        CI_LOGD("MESSAGE SENT TO JAVA: %d %s", what, body.c_str());
#endif
        
        callVoidMethodOnJavaListener("receiveMessageFromSketch", "(ILjava/lang/String;)V", what, getJNIEnv()->NewStringUTF(body.c_str()));
    }
    
    void CinderDelegate::sendMessageToSketch(int what, const string &body)
    {
#ifdef DEBUG_MESSAGES
        CI_LOGD("MESSAGE RECEIVED FROM JAVA: %d %s", what, body.c_str());
#endif
        
        sketch->sendMessage(Message(what, body));
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
    
    // ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    /*
     * REFERENCES:
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
    
    int CinderDelegate::getDisplayRotation()
    {
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        jmethodID getRotationMethod = env->GetMethodID(env->GetObjectClass(mJavaDisplay), "getRotation", "()I");
        return env->CallIntMethod(mJavaDisplay, getRotationMethod);
    }
    
    void CinderDelegate::pollSensorEvents()
    {
        ASensorEvent event;
        
        while (ASensorEventQueue_getEvents(mSensorEventQueue, &event, 1) > 0)
        {
            if (event.type == ASENSOR_TYPE_ACCELEROMETER)
            {
                Vec3f transformed;
                canonicalToWorld(getDisplayRotation(), (float*)&event.acceleration.v, transformed);
                
                /*
                 * ADDITIONAL TRANSFORMATION: FOR CONSISTENCY WITH iOS
                 */
                accelerated(-transformed.x / GRAVITY_EARTH, -transformed.y / GRAVITY_EARTH, transformed.z / GRAVITY_EARTH);
            }
        }
    }
    
    void CinderDelegate::accelerated(float x, float y, float z)
    {
        Vec3f acceleration(x, y, z);
        Vec3f filtered = mLastAccel * (1 - mAccelFilterFactor) + acceleration * mAccelFilterFactor;
        
        AccelEvent event(filtered, acceleration, mLastAccel, mLastRawAccel);
        sketch->accelerated(event);
        
        mLastAccel = filtered;
        mLastRawAccel = acceleration;
    }

    // ---------------------------------------- JNI ----------------------------------------
    
    JNIEnv* CinderDelegate::getJNIEnv()
    {
        JNIEnv *env = nullptr;
        
        int err = mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (err == JNI_EDETACHED)
        {
            CI_LOGE("getJNIEnv error: current thread not attached to Java VM");
        }
        else if (err == JNI_EVERSION)
        {
            CI_LOGE("getJNIEnv error: VM doesn't support requested JNI version");
        }
        
        return env;
    }
    
    void CinderDelegate::callVoidMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        env->CallVoidMethodV(mJavaListener, method, args);
        va_end(args);
    }
    
    jboolean CinderDelegate::callBooleanMethodOnJavaListener(const char *name, const char *sig, ...)
    {
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
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
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
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
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
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
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
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
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
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
        JNIEnv *env;
        mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        jclass cls = env->GetObjectClass(mJavaListener);
        jmethodID method = env->GetMethodID(cls, name, sig);
        
        va_list args;
        va_start(args, sig);
        jdouble ret = env->CallDoubleMethod(mJavaListener, method, args);
        va_end(args);
        
        return ret;
    }
}
