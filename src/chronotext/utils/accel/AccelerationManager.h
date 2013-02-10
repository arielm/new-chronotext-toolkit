/*
 * TRANSITIONAL SOLUTION FOR THE ACCELEROMETER,
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
    void enable(AccelerationDelegate *delegate, float updateFrequency) {}
    void disable() {}
};

#endif
