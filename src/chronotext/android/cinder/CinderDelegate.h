/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include <boost/asio.hpp>

#include <jni.h>
#include <android/sensor.h>

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
        static bool LOG_VERBOSE;
        
        CinderDelegate();
        virtual ~CinderDelegate() {}

        CinderSketch* getSketch();

        virtual void sketchCreated(CinderSketch *sketch) {}
        virtual void sketchDestroyed(CinderSketch *sketch) {}
        
        virtual void action(int actionId);
        virtual void receiveMessageFromSketch(int what, const std::string &body);
        virtual void sendMessageToSketch(int what, const std::string &body = "");
        
        void prelaunch(JNIEnv *env, jobject javaContext, jobject javaListener, jobject javaDisplay, int displayWidth, int displayHeight, float displayDensity);
        
        void setup(int width, int height);
        void shutdown();

        void resize(int width, int height);
        void event(int eventId);

        void draw();
        
        double getElapsedSeconds() const;
        uint32_t getElapsedFrames() const;

        bool isEmulated() const;
        WindowInfo getWindowInfo() const;
        DisplayInfo getDisplayInfo() const;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();

        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);

        // ---
        
        void callVoidMethodOnJavaListener(const char *name, const char *sig, ...);
        jboolean callBooleanMethodOnJavaListener(const char *name, const char *sig, ...);
        jchar callCharMethodOnJavaListener(const char *name, const char *sig, ...);
        jint callIntMethodOnJavaListener(const char *name, const char *sig, ...);
        jlong callLongMethodOnJavaListener(const char *name, const char *sig, ...);
        jfloat callFloatMethodOnJavaListener(const char *name, const char *sig, ...);
        jdouble callDoubleMethodOnJavaListener(const char *name, const char *sig, ...);
        jobject callObjectMethodOnJavaListener(const char *name, const char *sig, ...);
        
    protected:
        CinderSketch *sketch;

        jobject mJavaContext;
        jobject mJavaListener;
        jobject mJavaDisplay;
        
        DisplayInfo displayInfo;
        WindowInfo windowInfo;

        std::shared_ptr<ci::android::dostream> mOutputStream;
        
        ci::Timer mTimer;
        uint32_t mFrameCount;
        
        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        ASensorManager *mSensorManager;
        const ASensor *mAccelerometerSensor;
        ASensorEventQueue *mSensorEventQueue;
        
        AccelEvent::Filter accelFilter;

        void setSketch(CinderSketch *sketch);

        void start(int flags);
        void stop(int flags);
        
        void startIOService();
        void stopIOService();
        void pollIOService();
        
        void createSensorEventQueue();
        void destroySensorEventQueue();
        void pollSensorEvents();

        void handleAcceleration(ASensorEvent event);
        int getDisplayRotation();
    };
}
