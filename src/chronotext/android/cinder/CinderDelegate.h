/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/system/SystemInfo.h"

#include "cinder/Json.h"

#include <boost/asio.hpp>

#include <android/sensor.h>

#include <atomic>

namespace chr
{
    class CinderDelegate
    {
        /*
         * PARALLEL TO android/cinder/CinderRenderer.java
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
         * PARALLEL TO android/cinder/CinderDelegate.java
         */
        enum
        {
            ACTION_CAPTURE_BACK_KEY = 1,
            ACTION_RELEASE_BACK_KEY = 2
        };
        
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;
        
        CinderDelegate();
        virtual ~CinderDelegate() {}

        CinderSketch* getSketch();

        void sketchCreated(CinderSketch *sketch) {}
        void sketchDestroyed(CinderSketch *sketch) {}

        void event(int eventId);
        void action(int actionId);
        
        void receiveMessageFromSketch(int what, const std::string &body);
        void sendMessageToSketch(int what, const std::string &body = "");
        
        /*
         * INVOKED ON THE MAIN-THREAD, BEFORE RENDERER'S THREAD IS CREATED
         */
        void init(JNIEnv *env, jobject androidContext, jobject androidDisplay, int displayWidth, int displayHeight, float displayDensity);
        
        void launch(JNIEnv *env); // INVOKED ON THE RENDERER'S THREAD, BEFORE GL-CONTEXT IS CREATED
        void setup(int width, int height); // INVOKED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
        void shutdown();
        
        void resize(int width, int height);
        void draw();
        
        double getElapsedSeconds() const;
        uint32_t getElapsedFrames() const;

        bool isEmulated() const;
        const WindowInfo& getWindowInfo() const;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();

        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);

        // ---
        
        /*
         * TODO: MOVE TO SystemManager
         */
        
        ci::JsonTree jsonQuery(const char *methodName);
        
        void callVoidMethodOnListener(const char *name, const char *sig, ...);
        jboolean callBooleanMethodOnListener(const char *name, const char *sig, ...);
        jchar callCharMethodOnListener(const char *name, const char *sig, ...);
        jint callIntMethodOnListener(const char *name, const char *sig, ...);
        jlong callLongMethodOnListener(const char *name, const char *sig, ...);
        jfloat callFloatMethodOnListener(const char *name, const char *sig, ...);
        jdouble callDoubleMethodOnListener(const char *name, const char *sig, ...);
        jobject callObjectMethodOnListener(const char *name, const char *sig, ...);
        
    protected:
        CinderSketch *sketch;

        system::BootInfo bootInfo;
        WindowInfo windowInfo;

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
