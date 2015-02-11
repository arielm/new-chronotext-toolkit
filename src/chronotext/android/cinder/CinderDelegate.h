/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_ANDROID)
#   error UNSUPPORTED PLATFORM
#endif

// ---

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
         * INVOKED ON THE MAIN-THREAD
         *
         * RENDERER'S THREAD IS NOT AVAILABLE (EITHER NOT CREATED YET, OR ALREADY DESTROYED)
         */
        bool performInit(JNIEnv *env, jobject androidContext, jobject androidDisplay, const ci::Vec2i &displaySize, float displayDensity);
        void performUninit(JNIEnv *env);
        
        /*
         * INVOKED ON THE RENDERER'S THREAD
         *
         * GL-CONTEXT IS VALID
         */
        void performSetup(JNIEnv *env, const ci::Vec2i &size);
        void performShutdown(JNIEnv *env);
        
        void performResize(const ci::Vec2i &size);
        void performUpdate();
        void performDraw();

        void addTouch(int index, float x, float y);
        void updateTouch(int index, float x, float y);
        void removeTouch(int index, float x, float y);
        
        void messageFromBridge(int what, const std::string &body = "") final;
        void sendMessageToBridge(int what, const std::string &body = "") final;
        
        void handleEvent(int eventId) final;
        void performAction(int actionId) final;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;
        
        ci::JsonTree jsonQuery(const char *methodName) final;

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
