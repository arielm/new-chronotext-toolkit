/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

namespace chr
{
    class CinderDelegateBase
    {
    public:
        virtual ~CinderDelegateBase() {}
        
        virtual void messageFromBridge(int what, const std::string &body = "") = 0;
        virtual void sendMessageToBridge(int what, const std::string &body = "") {}
        
        virtual void handleEvent(int eventId) {}
        virtual void performAction(int actionId) {}
        
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
        virtual bool isEmulated() const = 0;
        virtual const WindowInfo& getWindowInfo() const = 0;
        
    protected:
        virtual void sketchCreated(CinderSketch *sketch) {}
        virtual void sketchDestroyed(CinderSketch *sketch) {}
    };
}
