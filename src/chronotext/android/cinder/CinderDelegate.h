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
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;

        /*
         * INVOKED ON THE MAIN-THREAD, BEFORE RENDERER'S THREAD IS CREATED
         */
        bool init(JNIEnv *env, jobject androidContext, jobject androidDisplay, const ci::Vec2i &displaySize, float displayDensity);
        
        void launch(); // INVOKED ON THE RENDERER'S THREAD, BEFORE GL-CONTEXT IS CREATED
        void setup(const ci::Vec2i &size); // INVOKED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
        void shutdown(JNIEnv *env);
        
        void resize(const ci::Vec2i &size);
        void update();
        void draw();

        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);
        
        void messageFromBridge(int what, const std::string &body = "") final;
        void sendMessageToBridge(int what, const std::string &body = "") final;
        
        void handleEvent(int eventId) final;
        void performAction(int actionId) final;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;

    protected:
        int updateCount = 0;

        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        ASensorManager *sensorManager;
        const ASensor *accelerometerSensor;
        ASensorEventQueue *sensorEventQueue;
        
        void startIOService();
        void stopIOService();
        
        void createSensorEventQueue();
        void destroySensorEventQueue();
        void pollSensorEvents();

        void handleAcceleration(ASensorEvent event);
        int getDisplayRotation();
    };
}
