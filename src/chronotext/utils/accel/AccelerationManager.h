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
