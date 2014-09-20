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
        enum
        {
            EVENT_RESUMED = 1,
            EVENT_SHOWN = 2,
            EVENT_PAUSED = 3,
            EVENT_HIDDEN = 4,
            EVENT_CONTEXT_LOST = 5,
            EVENT_CONTEXT_RENEWED = 6,
            EVENT_BACKGROUND = 7,
            EVENT_FOREGROUND = 8,
            EVENT_BACK_KEY = 9
        };
        
    public:
        JavaVM *mJavaVM;
        jobject mJavaContext;
        jobject mJavaListener;
        jobject mJavaDisplay;
        
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
        
        void launch(JavaVM *javaVM, jobject javaContext, jobject javaListener, jobject javaDisplay);
        
        void setup(int width, int height, float diagonal, float density);
        void shutdown();
        
        void resize();
        void draw();
        
        void event(int eventId);
        
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
        float getWindowContentScale() const;
        WindowInfo getWindowInfo() const;
        
        virtual void action(int actionId);
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
        
    protected:
        std::shared_ptr<ci::android::dostream> mOutputStream;
        
        WindowInfo mWindowInfo;
        
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
        
        void start(int flags);
        void stop(int flags);
        
        int getDisplayRotation();
        void processSensorEvents();
        void accelerated(float x, float y, float z);
    };
}

namespace chr = chronotext;
