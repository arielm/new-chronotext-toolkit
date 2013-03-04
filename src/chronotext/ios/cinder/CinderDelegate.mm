#import "CinderDelegate.h"
#import "GLViewController.h"

#include "chronotext/utils/accel/AccelEvent.h"

using namespace ci;
using namespace app;
using namespace std;

@implementation CinderDelegate

@synthesize view;
@synthesize viewController;
@synthesize sketch;
@synthesize accelFilterFactor = mAccelFilterFactor;
@synthesize width = mWidth;
@synthesize height = mHeight;
@synthesize contentScale = mContentScale;
@synthesize initialized = mInitialized;
@synthesize active = mActive;

- (id) init
{
    if (self = [super init])
    {
        mLastAccel = mLastRawAccel = Vec3f::zero();
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
    }
    
    return self;
}

- (void) dealloc
{
    sketch->shutdown();
	delete sketch;

    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [super dealloc];
}

- (void) startWithReason:(int)reason
{
    mFrameCount = 0;
    mTimer.start();
    
    if (reason == REASON_VIEW_WILL_APPEAR)
    {
        sketch->start(CinderSketch::FLAG_FOCUS_GAIN);
        mActive = YES;
    }
    else
    {
        sketch->start(CinderSketch::FLAG_APP_RESUME);
    }
}

- (void) stopWithReason:(int)reason
{
    mTimer.stop();

    if (reason == REASON_VIEW_WILL_DISAPEAR)
    {
        sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
        mActive = NO;
    }
    else
    {
        sketch->stop(CinderSketch::FLAG_APP_PAUSE);
    }
}

- (void) setup
{
    int mx;
    int my;
    
    switch (viewController.interfaceOrientation)
    {
        case UIInterfaceOrientationLandscapeLeft:
        case UIInterfaceOrientationLandscapeRight:
            mx = 0;
            my = 1;
            break;
            
        case UIInterfaceOrientationPortrait:
        case UIInterfaceOrientationPortraitUpsideDown:
            mx = 1;
            my = 0;
            break;
    }
    
    int frameWidth = view.frame.size.width;
    int frameHeight = view.frame.size.height;
    
    mWidth = mx * frameWidth + my * frameHeight;
    mHeight = mx * frameHeight + my * frameWidth;
    
    mContentScale = view.contentScaleFactor;

    sketch->setup(false);
    sketch->resize();
    mInitialized = YES;
}

- (void) update
{
    sketch->run(); // NECESSARY FOR THE "MESSAGE-PUMP"
    sketch->update();
    mFrameCount++;
}

- (void) draw
{
    sketch->draw();
}

- (double) elapsedSeconds
{
    return mTimer.getSeconds();
}

- (uint32_t) elapsedFrames
{
    return mFrameCount;
}

- (void) receiveMessageFromSketch:(int)what body:(NSString*)body
{}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    sketch->sendMessage(Message(what, shared_ptr<string>(new string([body UTF8String]))));
}

#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
    Vec3f direction(acceleration.x, acceleration.y, acceleration.z);
    Vec3f filtered = mLastAccel * (1 - mAccelFilterFactor) + direction * mAccelFilterFactor;

    AccelEvent event(filtered, direction, mLastAccel, mLastRawAccel);
    sketch->accelerated(event);
    
    mLastAccel = filtered;
    mLastRawAccel = direction;
}

#pragma mark ---------------------------------------- TOUCH ----------------------------------------

- (uint32_t) addTouchToMap:(UITouch*)touch
{
    uint32_t candidateId = 0;
    bool found = true;
    while (found)
    {
        candidateId++;
        found = false;
        for (map<UITouch*,uint32_t>::const_iterator mapIt = mTouchIdMap.begin(); mapIt != mTouchIdMap.end(); ++mapIt)
        {
            if (mapIt->second == candidateId)
            {
                found = true;
                break;
            }
        }
    }
    
    mTouchIdMap.insert(make_pair(touch, candidateId));
    return candidateId;
}

- (void) removeTouchFromMap:(UITouch*)touch
{
    map<UITouch*,uint32_t>::iterator found(mTouchIdMap.find(touch));
    if (found == mTouchIdMap.end())
    {
        cout << "Couldn' find touch in map?" << endl;
    }
    else
    {
        mTouchIdMap.erase(found);
    }
}

- (uint32_t) findTouchInMap:(UITouch*)touch
{
    map<UITouch*,uint32_t>::const_iterator found(mTouchIdMap.find(touch));
    if (found == mTouchIdMap.end())
    {
        cout << "Couldn' find touch in map?" << endl;
        return 0;
    }
    else
    {
        return found->second;
    }
}

- (void) updateActiveTouches
{
    static float contentScale = 1;
    
    vector<TouchEvent::Touch> activeTouches;
    for (map<UITouch*,uint32_t>::const_iterator touchIt = mTouchIdMap.begin(); touchIt != mTouchIdMap.end(); ++touchIt)
    {
        CGPoint pt = [touchIt->first locationInView:view];
        CGPoint prevPt = [touchIt->first previousLocationInView:view];
        activeTouches.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * contentScale, Vec2f(prevPt.x, prevPt.y) * contentScale, touchIt->second, [touchIt->first timestamp], touchIt->first));
    }
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    static float contentScale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * contentScale, Vec2f(prevPt.x, prevPt.y) * contentScale, [self addTouchToMap:touch], [touch timestamp], touch));
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesBegan(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    static float contentScale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];            
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * contentScale, Vec2f(prevPt.x, prevPt.y) * contentScale, [self findTouchInMap:touch], [touch timestamp], touch));
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesMoved(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    static float contentScale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * contentScale, Vec2f(prevPt.x, prevPt.y) * contentScale, [self findTouchInMap:touch], [touch timestamp], touch));
        [self removeTouchFromMap:touch];
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesEnded(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self touchesEnded:touches withEvent:event];
}

#pragma mark ---------------------------------------- BACKGROUND / FOREGROUND ----------------------------------------

- (void) applicationWillResignActive
{
    if (mInitialized && !mActive)
    {
        sketch->event(CinderSketch::EVENT_BACKGROUND);
    }
}

- (void) applicationDidBecomeActive
{
    if (mInitialized && !mActive)
    {
        sketch->event(CinderSketch::EVENT_FOREGROUND);
    }
}

@end
