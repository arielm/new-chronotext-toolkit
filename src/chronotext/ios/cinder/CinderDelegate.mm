/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * "TOUCH MAPPING" CODE FROM CINDER:
 * https://github.com/cinder/Cinder/blob/c894b2a81eb4d859070b177f989f60b470e92b8c/src/cinder/app/CinderViewCocoaTouch.mm
 */

#import "CinderDelegate.h"
#import "GLViewController.h"

#include "chronotext/utils/accel/AccelEvent.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

@implementation CinderDelegate

@synthesize view;
@synthesize viewController;
@synthesize sketch;
@synthesize accelFilterFactor;
@synthesize io;
@synthesize width;
@synthesize height;
@synthesize contentScale;
@synthesize initialized;
@synthesize active;

- (id) init
{
    if (self = [super init])
    {
        lastAccel = lastRawAccel = Vec3f::zero();
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
    }
    
    return self;
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    io->stop();

    sketch->shutdown();
    delete sketch;
    
    [super dealloc];
}

- (void) startWithReason:(int)reason
{
    frameCount = 0;
    timer.start();
    
    if (reason == REASON_VIEW_WILL_APPEAR)
    {
        sketch->start(CinderSketch::FLAG_FOCUS_GAINED);
        active = YES;
    }
    else
    {
        sketch->start(CinderSketch::FLAG_APP_RESUMED);
    }
}

- (void) stopWithReason:(int)reason
{
    timer.stop();

    if (reason == REASON_VIEW_WILL_DISAPPEAR)
    {
        sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
        active = NO;
    }
    else
    {
        sketch->stop(CinderSketch::FLAG_APP_PAUSED);
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
    
    width = mx * frameWidth + my * frameHeight;
    height = mx * frameHeight + my * frameWidth;
    
    contentScale = view.contentScaleFactor;
    
    // ---
    
    io = make_shared<boost::asio::io_service>();
    ioWork = make_shared<boost::asio::io_service::work>(*io);

    sketch->setIOService(*io);
    sketch->setup(false);
    sketch->resize();
    
    initialized = YES;
}

- (void) update
{
    io->poll();
    sketch->update();
    frameCount++;
}

- (void) draw
{
    sketch->draw();
}

- (double) elapsedSeconds
{
    return timer.getSeconds();
}

- (uint32_t) elapsedFrames
{
    return frameCount;
}

- (void) sendMessageToSketch:(int)what
{
    sketch->sendMessage(Message(what));
}

- (void) sendMessageToSketch:(int)what json:(id)json
{
    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:0 error:nil];
    NSString *string = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    
    [self sendMessageToSketch:what body:string];
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    sketch->sendMessage(Message(what, [body UTF8String]));
}

- (void) receiveMessageFromSketch:(int)what body:(NSString*)body
{}

#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
    Vec3f direction(acceleration.x, acceleration.y, acceleration.z);
    Vec3f filtered = lastAccel * (1 - accelFilterFactor) + direction * accelFilterFactor;

    AccelEvent event(filtered, direction, lastAccel, lastRawAccel);
    sketch->accelerated(event);
    
    lastAccel = filtered;
    lastRawAccel = direction;
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
        for (map<UITouch*,uint32_t>::const_iterator mapIt = touchIdMap.begin(); mapIt != touchIdMap.end(); ++mapIt)
        {
            if (mapIt->second == candidateId)
            {
                found = true;
                break;
            }
        }
    }
    
    touchIdMap.insert(make_pair(touch, candidateId));
    return candidateId;
}

- (void) removeTouchFromMap:(UITouch*)touch
{
    map<UITouch*,uint32_t>::iterator found(touchIdMap.find(touch));
    if (found != touchIdMap.end())
    {
        touchIdMap.erase(found);
    }
}

- (uint32_t) findTouchInMap:(UITouch*)touch
{
    map<UITouch*,uint32_t>::const_iterator found(touchIdMap.find(touch));
    if (found != touchIdMap.end())
    {
        return found->second;
    }
    
    return 0;
}

- (void) updateActiveTouches
{
    const float scale = 1;
    
    vector<TouchEvent::Touch> activeTouches;
    for (map<UITouch*,uint32_t>::const_iterator touchIt = touchIdMap.begin(); touchIt != touchIdMap.end(); ++touchIt)
    {
        CGPoint pt = [touchIt->first locationInView:view];
        CGPoint prevPt = [touchIt->first previousLocationInView:view];
        activeTouches.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, touchIt->second, [touchIt->first timestamp], touchIt->first));
    }
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    const float scale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self addTouchToMap:touch], [touch timestamp], touch));
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesBegan(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    const float scale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];            
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self findTouchInMap:touch], [touch timestamp], touch));
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesMoved(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    const float scale = 1;
    
    vector<TouchEvent::Touch> touchList;
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.push_back(TouchEvent::Touch(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self findTouchInMap:touch], [touch timestamp], touch));
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
    if (initialized && !active)
    {
        sketch->event(CinderSketch::EVENT_BACKGROUND);
    }
}

- (void) applicationDidBecomeActive
{
    if (initialized && !active)
    {
        sketch->event(CinderSketch::EVENT_FOREGROUND);
    }
}

@end
