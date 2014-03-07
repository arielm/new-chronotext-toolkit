/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <jni.h>
#include <android/sensor.h>

#include "chronotext/cinder/CinderSketch.h"

#include "cinder/android/LogStream.h"

namespace chronotext
{
    class CinderDelegate
    {
        std::shared_ptr<ci::android::dostream> mOutputStream;
        
        int mWidth;
        int mHeight;
        int mDisplayRotation;
        
        ci::Timer mTimer;
        uint32_t mFrameCount;
        
        float mAccelFilterFactor;
        ci::Vec3f mLastAccel, mLastRawAccel;
        
        ASensorManager *mSensorManager;
        const ASensor *mAccelerometerSensor;
        ASensorEventQueue *mSensorEventQueue;
        
        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
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
        jobject mJavaContext;
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
        
        void launch(JavaVM *javaVM, jobject javaContext, jobject javaListener);
        
        void setup(int width, int height, int displayRotation);
        void shutdown();
        
        void draw();
        void event(int id);
        
        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);
        
        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();
        
        std::ostream& console();
        boost::asio::io_service& io_service() const;

        double getElapsedSeconds() const;
        uint32_t getElapsedFrames() const;
        
        int getWindowWidth() const;
        int getWindowHeight() const;
        ci::Vec2f getWindowCenter() const;
        ci::Vec2i getWindowSize() const;
        float getWindowAspectRatio() const;
        ci::Area getWindowBounds() const;
        float getWindowDensity() const;
        
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
}

namespace chr = chronotext;
