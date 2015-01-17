/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderDelegateBase.h"
#include "chronotext/system/SystemInfo.h"

#include <boost/asio.hpp>

#include <android/sensor.h>

#include <atomic>

namespace chr
{
    class CinderDelegate : public CinderDelegateBase
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
            EVENT_BACK_PRESSED = 9
        };
        
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;

        /*
         * INVOKED ON THE MAIN-THREAD, BEFORE RENDERER'S THREAD IS CREATED
         */
        void init(jobject androidContext, jobject androidDisplay, int displayWidth, int displayHeight, float displayDensity);
        
        void launch(); // INVOKED ON THE RENDERER'S THREAD, BEFORE GL-CONTEXT IS CREATED
        void setup(int width, int height); // INVOKED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
        void shutdown();
        
        void resize(int width, int height);
        void draw();

        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);

        void messageFromBridge(int what, const std::string &body = "") final;
        void sendMessageToBridge(int what, const std::string &body = "") final;
        
        void eventFromBridge(int eventId) final;
        void performAction(int actionId) final;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();

        bool isEmulated() const;
        const WindowInfo& getWindowInfo() const;

        double elapsedSeconds() const;
        uint32_t elapsedFrames() const;

    protected:
        CinderSketch *sketch = nullptr;

        ci::Timer timer;
        int frameCount = 0;
        
        system::InitInfo initInfo;
        WindowInfo windowInfo;

        AccelEvent::Filter accelFilter;

        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        ASensorManager *sensorManager;
        const ASensor *accelerometerSensor;
        ASensorEventQueue *sensorEventQueue;

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
