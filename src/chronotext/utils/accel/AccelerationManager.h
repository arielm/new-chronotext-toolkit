/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TRANSITIONAL SOLUTION FOR CINDER 0.8.5,
 * UNTIL POPULAR DEVICES WITHOUT GYROSCOPES
 * (E.G. 3GS AND IPAD-1) ARE GONE...
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA_TOUCH)

#include "AccelerationManagerImplCocoaTouch.h"
typedef AccelerationManagerImplCocoaTouch AccelerationManager;

#else

#include "AccelEvent.h"

class AccelerationDelegate
{
public:
    virtual void accelerated(AccelEvent event) {}
};

class AccelerationManager
{
public:
    void enable(AccelerationDelegate *delegate, float updateFrequency = 30, float filterFactor = 0.1f) {}
    void disable() {}
};

#endif
