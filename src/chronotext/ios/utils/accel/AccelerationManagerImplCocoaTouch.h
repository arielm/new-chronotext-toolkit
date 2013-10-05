/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "AccelEvent.h"

#if defined(__OBJC__)
@class CocoaProxy;
#else
class CocoaProxy;
#endif

class AccelerationDelegate
{
public:
    virtual void accelerated(AccelEvent event) {}
};

class AccelerationManagerImplCocoaTouch
{
    float mAccelFilterFactor;
    ci::Vec3f mLastAccel, mLastRawAccel;
    
    CocoaProxy *mProxy;
    AccelerationDelegate *mDelegate;
    
public:
    AccelerationManagerImplCocoaTouch();
    ~AccelerationManagerImplCocoaTouch();

    void enable(AccelerationDelegate *delegate, float updateFrequency = 30, float filterFactor = 0.1f);
    void disable();
    
    void accelerated(const ci::Vec3f &acceleration);
};
