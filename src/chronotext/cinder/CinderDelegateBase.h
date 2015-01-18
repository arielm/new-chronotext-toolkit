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

namespace chr
{
    class CinderDelegateBase
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;

        virtual ~CinderDelegateBase() {}
        
        virtual void messageFromBridge(int what, const std::string &body = "");
        virtual void sendMessageToBridge(int what, const std::string &body = "") {}
        
        virtual void handleEvent(int eventId) {}
        virtual void performAction(int actionId) {}
        
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
        /*
         * FIXME: OVER-COMPLICATED (REASON: THE FOLLOWING METHODS ARE NOT DECLARED ON iOS)
         */
        virtual int getCode(const ci::app::KeyEvent &keyEvent) const { return 0; }
        virtual bool isShiftDown(const ci::app::KeyEvent &keyEvent) const { return false; }
        virtual bool isAltDown(const ci::app::KeyEvent &keyEvent) const { return false; }
        virtual bool isAccelDown(const ci::app::KeyEvent &keyEvent) const { return false; }
        
    protected:
        CinderSketch *sketch = nullptr;

        system::InitInfo initInfo;
        system::LaunchInfo launchInfo;
        system::SetupInfo setupInfo;
        
        AccelEvent::Filter accelFilter;

        virtual void sketchCreated(CinderSketch *sketch) {}
        virtual void sketchDestroyed(CinderSketch *sketch) {}
        
        bool _init();
        void _launch();
        void _setup();
        void _shutdown();
    };
}
