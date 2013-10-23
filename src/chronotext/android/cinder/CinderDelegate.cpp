/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/FileSystem.h"
#include "chronotext/utils/accel/AccelEvent.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace std;
using namespace ci;
using namespace app;

#define GRAVITY_EARTH 9.80665f

namespace chronotext
{
    /*
     * CALLED ON THE RENDERER'S THREAD FROM chronotext.android.gl.GLRenderer.onSurfaceCreated()
     */
    void CinderDelegate::launch(JavaVM *javaVM, jobject javaContext, jobject javaListener)
    {
        mJavaVM = javaVM;
        mJavaListener = javaListener;
        
        JNIEnv *env;
        javaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        // ---
        
        jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getAssets", "()Landroid/content/res/AssetManager;");
        AAssetManager *assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(javaContext, getAssetsMethod));
        
        FileSystem::setAndroidAssetManager(assetManager);
        
        // ---
        
        jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getFilesDir", "()Ljava/io/File;");
        jobject filesDirObject = env->CallObjectMethod(javaContext, getFilesDirMethod);
        jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
        jstring absolutePathString = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
        
        const char *internalDataPath = env->GetStringUTFChars(absolutePathString, NULL);
        FileSystem::setAndroidInternalDataPath(internalDataPath);
        env->ReleaseStringUTFChars(absolutePathString, internalDataPath);
        
        // ---
        
        jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(javaContext), "getPackageCodePath", "()Ljava/lang/String;");
        jstring packageCodePathString = (jstring)env->CallObjectMethod(javaContext, getPackageCodePathMethod);
        
        const char *apkPath = env->GetStringUTFChars(packageCodePathString, NULL);
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
        mSensorEventQueue = ASensorManager_createEventQueue(mSensorManager, looper, 3, NULL, NULL/*sensorEventCallback, this*/); // WOULD BE BETTER TO USE A CALL-BACK, BUT IT'S NOT WORKING
    }
    
    void CinderDelegate::processSensorEvents()
    {
        ASensorEvent event;
        
        while (ASensorEventQueue_getEvents(mSensorEventQueue, &event, 1) > 0)
        {
            if (event.type == ASENSOR_TYPE_ACCELEROMETER)
            {
                float x = event.acceleration.x;
                float y = event.acceleration.y;
                float z = event.acceleration.z;
                
                /*
                 * APPLYING THE EVENTUAL ORIENTATION FIX
                 */
                if (mAccelerometerRotation == ACCELEROMETER_ROTATION_LANDSCAPE)
                {
                    float tmp = x;
                    x = -y;
                    y = +tmp;
                }
                else if (mAccelerometerRotation == ACCELEROMETER_ROTATION_PORTRAIT)
                {
                    float tmp = x;
                    x = +y;
                    y = -tmp;
                }
                
                /*
                 * FOR CONSISTENCY WITH iOS
                 */
                x /= -GRAVITY_EARTH;
                y /= +GRAVITY_EARTH;
                z /= +GRAVITY_EARTH;
                
                accelerated(x, y, z);
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
    
    void CinderDelegate::setup(int width, int height, int accelerometerRotation)
    {
        mWidth = width;
        mHeight = height;
        mAccelerometerRotation = accelerometerRotation;
        
        io = make_shared<boost::asio::io_service>();
        ioWork = make_shared<boost::asio::io_service::work>(*io);
        
        sketch->setIOService(*io);
        sketch->setup(false);
        sketch->resize();
    }
    
    void CinderDelegate::shutdown()
    {
        ASensorManager_destroyEventQueue(mSensorManager, mSensorEventQueue);
        io->stop();
        
        sketch->shutdown();
        delete sketch;
    }
    
    void CinderDelegate::draw()
    {
        /*
         * WOULD BE BETTER TO USE A CALL-BACK, BUT IT'S NOT WORKING
         */
        processSensorEvents();
        
        io->poll();
        sketch->update();
        mFrameCount++;

        sketch->draw();
    }
    
    void CinderDelegate::event(int id)
    {
        switch (id)
        {
            case EVENT_ATTACHED:
            case EVENT_SHOWN:
                mFrameCount = 0;
                mTimer.start();
                sketch->start(CinderSketch::FLAG_FOCUS_GAINED);
                break;
                
            case EVENT_RESUMED:
                mFrameCount = 0;
                mTimer.start();
                
                /*
                 * ASSERTIONS: THE GL CONTEXT HAS JUST BEEN RE-CREATED, WITH THE SAME DIMENSIONS AS BEFORE
                 */
                sketch->setup(true);
                
                sketch->start(CinderSketch::FLAG_APP_RESUMED);
                break;
                
            case EVENT_DETACHED:
            case EVENT_HIDDEN:
                mTimer.stop();
                sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
                break;
                
            case EVENT_PAUSED:
                mTimer.stop();
                sketch->stop(CinderSketch::FLAG_APP_PAUSED);
                break;
                
            case EVENT_BACKGROUND:
                sketch->event(CinderSketch::EVENT_BACKGROUND);
                break;
                
            case EVENT_FOREGROUND:
                sketch->event(CinderSketch::EVENT_FOREGROUND);
                break;
        }
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
        return mTimer.getSeconds();
    }
    
    uint32_t CinderDelegate::getElapsedFrames() const
    {
        return mFrameCount;
    }
    
    int CinderDelegate::getWindowWidth() const
    {
        return mWidth;
    }
    
    int CinderDelegate::getWindowHeight() const
    {
        return mHeight;
    }
    
    Vec2f CinderDelegate::getWindowCenter() const
    {
        return Vec2f(mWidth * 0.5f, mHeight * 0.5f);
    }
    
    Vec2i CinderDelegate::getWindowSize() const
    {
        return Vec2i(mWidth, mHeight);
    }
    
    float CinderDelegate::getWindowAspectRatio() const
    {
        return mWidth / (float)mHeight;
    }
    
    Area CinderDelegate::getWindowBounds() const
    {
        return Area(0, 0, mWidth, mHeight);
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
    
    // ---------------------------------------- JNI ----------------------------------------
    
    JNIEnv* CinderDelegate::getJNIEnv()
    {
        JNIEnv *env = NULL;
        
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
