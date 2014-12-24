/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "cinder/Json.h"

#include <boost/asio.hpp>

#include <jni.h>
#include <android/sensor.h>

namespace chr
{
    class CinderDelegate
    {
        /*
         * PARALLEL TO org.chronotext.cinder.CinderRenderer.java
         */
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
        
        /*
         * PARALLEL TO org.chronotext.cinder.CinderDelegate.java
         */
        enum
        {
            ACTION_CAPTURE_BACK_KEY = 1,
            ACTION_RELEASE_BACK_KEY = 2
        };
        
    public:
        static bool VERBOSE;
        
        CinderDelegate();
        virtual ~CinderDelegate() {}

        CinderSketch* getSketch();

        void sketchCreated(CinderSketch *sketch) {}
        void sketchDestroyed(CinderSketch *sketch) {}

        void event(int eventId);
        void action(int actionId);
        
        void receiveMessageFromSketch(int what, const std::string &body);
        void sendMessageToSketch(int what, const std::string &body = "");
        
        void init(JNIEnv *env, jobject javaContext, jobject javaListener, jobject javaDisplay, int displayWidth, int displayHeight, float displayDensity);
        void setup(int width, int height);
        void shutdown();

        void resize(int width, int height);
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
        
        ci::JsonTree jsonQuery(const char *methodName);
        
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

        jobject javaContext_;
        jobject javaListener_;
        jobject javaDisplay_;
        
        DisplayInfo displayInfo_;
        WindowInfo windowInfo_;

        ci::Timer timer;
        uint32_t frameCount;
        
        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        ASensorManager *sensorManager;
        const ASensor *accelerometerSensor;
        ASensorEventQueue *sensorEventQueue;
        
        AccelEvent::Filter accelFilter;

        void setSketch(CinderSketch *sketch);

        void start(CinderSketch::Reason reason);
        void stop(CinderSketch::Reason reason);
        
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
