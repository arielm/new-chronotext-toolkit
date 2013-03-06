#pragma once

#include <jni.h>
#include <android/asset_manager.h>
#include <android/sensor.h>

#include "chronotext/cinder/CinderSketch.h"

#include "cinder/android/LogStream.h"

class CinderDelegate
{
    std::shared_ptr<ci::android::dostream> mOutputStream;

    int mWidth;
    int mHeight;
    int mAccelerometerRotation;

    ci::Timer mTimer;
    uint32_t mFrameCount;

	float mAccelFilterFactor;
	ci::Vec3f mLastAccel, mLastRawAccel;

	ASensorManager *mSensorManager;
	const ASensor *mAccelerometerSensor;
	ASensorEventQueue *mSensorEventQueue;

	static int sensorEventCallback(int fd, int events, void *data)
    {
	    CinderDelegate *instance = (CinderDelegate*)data;
	    instance->processSensorEvents();

	    return 1;
    }

	void processSensorEvents();
    void accelerated(float x, float y, float z);

    enum
    {
        ACCELEROMETER_ROTATION_DEFAULT = 0,
        ACCELEROMETER_ROTATION_PORTRAIT,
        ACCELEROMETER_ROTATION_LANDSCAPE
    };

    enum
    {
        EVENT_ATTACHED = 1,
        EVENT_DETACHED,
        EVENT_PAUSED,
        EVENT_RESUMED,
        EVENT_SHOWN,
        EVENT_HIDDEN,
    	EVENT_BACKGROUND,
    	EVENT_FOREGROUND
    };

public:
    JavaVM *mJavaVM;
    jobject mJavaListener;

    CinderSketch *sketch;
    
    CinderDelegate()
    :
    mLastAccel(ci::Vec3f::zero()),
    mLastRawAccel(ci::Vec3f::zero())
    {}

    virtual ~CinderDelegate()
    {
        CI_LOGD("CinderDelegate DELETED");
    }

    void launch(AAssetManager *assetManager, JavaVM *javaVM, jobject javaListener);

    void setup(int width, int height, int accelerometerRotation);
    void shutdown();

    void draw();
    void event(int id);

    void addTouch(float x, float y);
    void updateTouch(float x, float y);
    void removeTouch(float x, float y);
    
	void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f);
	void disableAccelerometer();

    double getElapsedSeconds() const;
    uint32_t getElapsedFrames() const;
    
    int getWindowWidth();
    int getWindowHeight();
    ci::Vec2f getWindowSize();
    float getWindowAspectRatio();
    ci::Area getWindowBounds() const;
    
    std::ostream& console();
    
    virtual void receiveMessageFromSketch(int what, const std::string &body);
    virtual void sendMessageToSketch(int what, const std::string &body);

    // ---------------------------------------- JNI ----------------------------------------

    JNIEnv* getJNIEnv();

    void callVoidMethodOnJavaListener(const char *name, const char *sig, ...);
    jboolean callBooleanMethodOnJavaListener(const char *name, const char *sig, ...);
    jchar callCharMethodOnJavaListener(const char *name, const char *sig, ...);
    jint callIntMethodOnJavaListener(const char *name, const char *sig, ...);
    jlong callLongMethodOnJavaListener(const char *name, const char *sig, ...);
    jfloat callFloatMethodOnJavaListener(const char *name, const char *sig, ...);
    jdouble callDoubleMethodOnJavaListener(const char *name, const char *sig, ...);
};
