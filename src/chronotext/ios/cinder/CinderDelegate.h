/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderDelegateBase.h"

#include "cinder/app/TouchEvent.h"

#include <boost/asio.hpp>

namespace chr
{
    class CinderDelegate : public CinderDelegateBase
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;

        bool init();
        void launch();
        void setup(const WindowInfo &windowInfo);
        void shutdown();
        
        void resize(const ci::Vec2i &size);
        void update();
        void draw();

        void touchesBegan(ci::app::TouchEvent event);
        void touchesMoved(ci::app::TouchEvent event);
        void touchesEnded(ci::app::TouchEvent event);

        void handleEvent(int eventId) final;

        void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;
        
        void handleAcceleration(const ci::Vec3f &acceleration);
        
    protected:
        int updateCount = 0;

        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        void startIOService();
        void stopIOService();
    };
}
