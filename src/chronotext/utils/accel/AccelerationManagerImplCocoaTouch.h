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
    ci::Vec3f mLastRawAccel;
    CocoaProxy *mProxy;
    AccelerationDelegate *mDelegate;
    
public:
    AccelerationManagerImplCocoaTouch();
    ~AccelerationManagerImplCocoaTouch();

    void enable(AccelerationDelegate *delegate, float updateFrequency = 30);
    void disable();
    
    void accelerated(const ci::Vec3f &acceleration);
};
