/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/utils/accel/AccelerationManagerImplCocoaTouch.h"

using namespace ci;

@interface CocoaProxy : NSObject <UIAccelerometerDelegate>
{
    AccelerationManagerImplCocoaTouch *mTarget;
}

- (id) initWithTarget:(AccelerationManagerImplCocoaTouch*)target;

@end

@implementation CocoaProxy

- (id) initWithTarget:(AccelerationManagerImplCocoaTouch*)target
{
    if (self = [super init])
    {
        mTarget = target;
        return self;
    }
    
    return nil;
}

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
    mTarget->accelerated(Vec3f(acceleration.x, acceleration.y, acceleration.z));
}

@end

AccelerationManagerImplCocoaTouch::AccelerationManagerImplCocoaTouch()
:
mDelegate(NULL),
mLastAccel(Vec3f::zero()),
mLastRawAccel(Vec3f::zero())
{
    mProxy = [[CocoaProxy alloc] initWithTarget:this];
}

AccelerationManagerImplCocoaTouch::~AccelerationManagerImplCocoaTouch()
{
    [mProxy release];
}

void AccelerationManagerImplCocoaTouch::enable(AccelerationDelegate *delegate, float updateFrequency, float filterFactor)
{
    mDelegate = delegate;
    mAccelFilterFactor = filterFactor;
    
    if (updateFrequency <= 0)
    {
        updateFrequency = 30;
    }
    
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1 / updateFrequency)];
    [[UIAccelerometer sharedAccelerometer] setDelegate:mProxy];
}

void AccelerationManagerImplCocoaTouch::disable()
{
    [[UIAccelerometer sharedAccelerometer] setDelegate:nil];
}

void AccelerationManagerImplCocoaTouch::accelerated(const Vec3f &acceleration)
{
    Vec3f filtered = mLastAccel * (1 - mAccelFilterFactor) + acceleration * mAccelFilterFactor;

    if (mDelegate)
    {
        mDelegate->accelerated(AccelEvent(filtered, acceleration, mLastAccel, mLastRawAccel));
    }
    
    mLastAccel = filtered;
    mLastRawAccel = acceleration;
}
