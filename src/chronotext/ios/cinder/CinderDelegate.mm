/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * "TOUCH MAPPING" BASED ON CINDER:
 * https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/app/CinderViewCocoaTouch.mm
 */

#import "CinderDelegate.h"
#import "GLViewController.h"

#include "chronotext/utils/accel/AccelEvent.h"
#include "chronotext/system/SystemInfo.h"

#include <map>

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

@interface CinderDelegate ()
{
    std::map<UITouch*, uint32_t> touchIdMap;
    
    float accelFilterFactor;
    ci::Vec3f lastAccel, lastRawAccel;
    
    std::shared_ptr<boost::asio::io_service> io;
    std::shared_ptr<boost::asio::io_service::work> ioWork;
    
    chr::WindowInfo windowInfo;
    
    ci::Timer timer;
    uint32_t frameCount;
    
    BOOL initialized;
    BOOL active;
}

- (void) updateDisplay;
- (void) updateWindow;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderDelegate

@synthesize view;
@synthesize viewController;
@synthesize sketch;
@synthesize accelFilterFactor;
@synthesize io;
@synthesize windowInfo;
@synthesize initialized;
@synthesize active;

- (id) init
{
    if (self = [super init])
    {
        lastAccel = lastRawAccel = Vec3f::zero();
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidReceiveMemoryWarningNotification) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
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
    sketch->clock().start();
    
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
    sketch->clock().stop();
    
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
    [self updateWindow];
    [self updateDisplay];
    
    // ---
    
    io = make_shared<boost::asio::io_service>();
    ioWork = make_shared<boost::asio::io_service::work>(*io);

    sketch->setIOService(*io);
    sketch->timeline().stepTo(0);
    
    sketch->setup(false);
    initialized = YES;
}

- (void) resize
{
    [self updateWindow];
    sketch->resize();
}

- (void) update
{
    sketch->clock().update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME
    io->poll();
    
    /*
     * MUST BE CALLED BEFORE Sketch::update
     * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
     *
     * NOTE THAT getTime() COULD HAVE BEEN ALREADY CALLED
     * WITHIN ONE OF THE PREVIOUSLY "POLLED" FUNCTIONS
     */
    double now = sketch->clock().getTime();
    
    sketch->update();
    sketch->timeline().stepTo(now);
    frameCount++;
}

- (void) draw
{
    if (frameCount == 0)
    {
        [self update]; // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
    }
    
    sketch->draw();
}

- (double) elapsedSeconds
{
    return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-VALUE PER FRAME
}

- (uint32_t) elapsedFrames
{
    return frameCount;
}

- (void) action:(int)actionId
{}

- (void) receiveMessageFromSketch:(int)what body:(NSString*)body
{}

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

- (void) updateWindow
{
    switch (viewController.interfaceOrientation)
    {
        case UIInterfaceOrientationLandscapeLeft:
        case UIInterfaceOrientationLandscapeRight:
            windowInfo.size.x = view.frame.size.height;
            windowInfo.size.y = view.frame.size.width;
            break;
            
        case UIInterfaceOrientationPortrait:
        case UIInterfaceOrientationPortraitUpsideDown:
            windowInfo.size.x = view.frame.size.width;
            windowInfo.size.y = view.frame.size.height;
            break;
    }
    
    windowInfo.size *= view.contentScaleFactor;
    windowInfo.contentScale = view.contentScaleFactor;
}

- (void) updateDisplay
{
    /*
     * TODO: HANDLE LATEST DEVICES
     */
    
    switch (SystemInfo::instance().getSizeFactor())
    {
        case SystemInfo::SIZE_FACTOR_PHONE:
            if (windowInfo.size.x == 1136)
            {
                windowInfo.diagonal = 4;
            }
            else
            {
                windowInfo.diagonal = 3.54f;
            }
            break;
            
        case SystemInfo::SIZE_FACTOR_TABLET:
            windowInfo.diagonal = 9.7f;
            break;
            
        case SystemInfo::SIZE_FACTOR_TABLET_MINI:
            windowInfo.diagonal = 7.9f;
            break;
    }
    
    windowInfo.density = windowInfo.size.length() / windowInfo.diagonal;
    
    // ---
    
    switch (view.drawableMultisample)
    {
        case GLKViewDrawableMultisampleNone:
            windowInfo.aaLevel = 0;
            break;
            
        case GLKViewDrawableMultisample4X:
            windowInfo.aaLevel = 4;
            break;
    }
}

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
        
        for (auto &it : touchIdMap)
        {
            if (it.second == candidateId)
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
    auto found = touchIdMap.find(touch);
    
    if (found != touchIdMap.end())
    {
        touchIdMap.erase(found);
    }
}

- (uint32_t) findTouchInMap:(UITouch*)touch
{
    auto found = touchIdMap.find(touch);
    
    if (found != touchIdMap.end())
    {
        return found->second;
    }
    
    return 0;
}

- (void) updateActiveTouches
{
    float scale = view.contentScaleFactor;;
    vector<TouchEvent::Touch> activeTouches;
    
    for (auto &it : touchIdMap)
    {
        CGPoint pt = [it.first locationInView:view];
        CGPoint prevPt = [it.first previousLocationInView:view];
        activeTouches.emplace_back(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, it.second, [it.first timestamp], it.first);
    }
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    float scale = view.contentScaleFactor;
    vector<TouchEvent::Touch> touchList;
    
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.emplace_back(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self addTouchToMap:touch], [touch timestamp], touch);
    }
    
    [self updateActiveTouches];
    if (!touchList.empty())
    {
        sketch->touchesBegan(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    float scale = view.contentScaleFactor;
    vector<TouchEvent::Touch> touchList;
    
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.emplace_back(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self findTouchInMap:touch], [touch timestamp], touch);
    }
    
    [self updateActiveTouches];
    
    if (!touchList.empty())
    {
        sketch->touchesMoved(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    const float scale = view.contentScaleFactor;
    vector<TouchEvent::Touch> touchList;
    
    for (UITouch *touch in touches)
    {
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        touchList.emplace_back(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, [self findTouchInMap:touch], [touch timestamp], touch);
        
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

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

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

- (void) applicationDidReceiveMemoryWarningNotification
{
    if (initialized)
    {
        sketch->event(CinderSketch::EVENT_MEMORY_WARNING);
    }
}

@end
