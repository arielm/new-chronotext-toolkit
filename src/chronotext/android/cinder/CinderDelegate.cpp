#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/InputSource.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define GRAVITY_EARTH 9.80665f

/*
 * CALLED ON THE RENDERER'S THREAD FROM chronotext.android.gl.GLRenderer.onSurfaceCreated()
 */
void CinderDelegate::launch(AAssetManager *assetManager, JavaVM *javaVM, jobject javaListener)
{
    mJavaVM = javaVM;
    mJavaListener = javaListener;

    // ---

    InputSource::setAndroidAssetManager(assetManager);

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

    sketch->setup(false);
    sketch->resize(ResizeEvent(Vec2i(mWidth, mHeight)));
}

void CinderDelegate::shutdown()
{
    ASensorManager_destroyEventQueue(mSensorManager, mSensorEventQueue);

    sketch->shutdown();
    delete sketch;
}

void CinderDelegate::draw()
{
    /*
     * WOULD BE BETTER TO USE A CALL-BACK, BUT IT'S NOT WORKING
     */
    processSensorEvents();

    sketch->run(); // NECESSARY FOR THE "MESSAGE-PUMP"
    sketch->update();
    sketch->draw();
    mFrameCount++;
}

void CinderDelegate::event(int id)
{
    switch (id)
    {
        case EVENT_ATTACHED:
        case EVENT_SHOWN:
	    mFrameCount = 0;
            mTimer.start();
            sketch->start(CinderSketch::FLAG_FOCUS_GAIN);
            break;
            
        case EVENT_RESUMED:
            mFrameCount = 0;
            mTimer.start();
            
            /*
             * ASSERTION: THE GL CONTEXT HAS JUST BEEN RE-CREATED
             */
            sketch->setup(true);
            sketch->resize(ResizeEvent(Vec2i(mWidth, mHeight)));
            
            sketch->start(CinderSketch::FLAG_APP_RESUME);
            break;
            
        case EVENT_DETACHED:
        case EVENT_HIDDEN:
            mTimer.stop();
            sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
            break;
            
        case EVENT_PAUSED:
            mTimer.stop();
            sketch->stop(CinderSketch::FLAG_APP_PAUSE);
            break;

        case EVENT_BACKGROUND:
        	sketch->event(CinderSketch::EVENT_BACKGROUND);
        	break;

        case EVENT_FOREGROUND:
        	sketch->event(CinderSketch::EVENT_FOREGROUND);
        	break;
    }
}

void CinderDelegate::addTouch(float x, float y)
{
    sketch->addTouch(0, x, y);
}

void CinderDelegate::updateTouch(float x, float y)
{
    sketch->updateTouch(0, x, y);
}

void CinderDelegate::removeTouch(float x, float y)
{
    sketch->removeTouch(0, x, y);
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

double CinderDelegate::getElapsedSeconds() const
{
    return mTimer.getSeconds();
}

uint32_t CinderDelegate::getElapsedFrames() const
{
    return mFrameCount;
}

int CinderDelegate::getWindowWidth()
{
    return mWidth;
}

int CinderDelegate::getWindowHeight()
{
    return mHeight;
}

Vec2f CinderDelegate::getWindowSize()
{
    return Vec2f(mWidth, mHeight);
}

float CinderDelegate::getWindowAspectRatio()
{
    return mWidth / (float)mHeight;
}

Area CinderDelegate::getWindowBounds() const
{
    return Area(0, 0, mWidth, mHeight);
}

ostream& CinderDelegate::console()
{
    if (!mOutputStream)
    {
        mOutputStream = shared_ptr<cinder::android::dostream>(new android::dostream);
    }
    
    return *mOutputStream;
}

void CinderDelegate::receiveMessageFromSketch(int what, const string &body)
{
    CI_LOGD("MESSAGE SENT TO JAVA: %d %s", what, body.c_str());
    callVoidMethodOnJavaListener("receiveMessageFromSketch", "(ILjava/lang/String;)V", what, getJNIEnv()->NewStringUTF(body.c_str()));
}

void CinderDelegate::sendMessageToSketch(int what, const string &body)
{
    CI_LOGD("MESSAGE RECEIVED FROM JAVA: %d %s", what, body.c_str());
    sketch->sendMessage(Message(what, boost::shared_ptr<string>(new string(body))));
}

// ---------------------------------------- JNI ----------------------------------------

JNIEnv* CinderDelegate::getJNIEnv()
{
    JNIEnv *env = NULL;

    int err = mJavaVM->GetEnv((void**) &env, JNI_VERSION_1_4);

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
