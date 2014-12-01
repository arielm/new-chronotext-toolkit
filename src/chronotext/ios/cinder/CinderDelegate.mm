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

#include "chronotext/system/SystemManager.h"
#include "chronotext/utils/accel/AccelEvent.h"

#include <map>

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

@interface CinderDelegate ()
{
    AccelEvent::Filter accelFilter;
    
    shared_ptr<boost::asio::io_service> io;
    shared_ptr<boost::asio::io_service::work> ioWork;

    DisplayInfo displayInfo;
    WindowInfo windowInfo;

    BOOL initialized;
    BOOL active;
    BOOL forceResize;

    Timer timer;
    uint32_t frameCount;
    
    map<UITouch*, uint32_t> touchIdMap;
}

- (void) startIOService;
- (void) stopIOService;
- (void) pollIOService;

- (void) updateDisplayInfo;
- (Vec2f) windowSize;
- (int) aaLevel;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderDelegate

@synthesize view;
@synthesize viewController;
@synthesize sketch;
@synthesize accelFilter;
@synthesize io;
@synthesize displayInfo;
@synthesize windowInfo;
@synthesize initialized;
@synthesize active;

- (id) init
{
    if (self = [super init])
    {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidReceiveMemoryWarningNotification) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
        
        // ---
        
        /*
         * TEMPORARY, UNTIL TRANSITION TO chr::context IS OVER
         */
        
        sketch = chr::createSketch();
        sketch->context = self;
    }
    
    return self;
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self stopIOService];
    
    sketch->shutdown();
    chr::destroySketch(sketch);
    
    [super dealloc];
}

- (void) startWithReason:(int)reason
{
    frameCount = 0;
    
    timer.start();
    sketch->clock().start();
    
    if (reason == REASON_VIEW_WILL_APPEAR)
    {
        sketch->start(CinderSketch::FLAG_APP_SHOWN);
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
        sketch->stop(CinderSketch::FLAG_APP_HIDDEN);
        active = NO;
    }
    else
    {
        sketch->stop(CinderSketch::FLAG_APP_PAUSED);
    }
}

- (void) setup
{
    /*
     * TODO: displayInfo SHOULD BE UPDATED AT LAUNCH-TIME
     */
    [self updateDisplayInfo];
    
    windowInfo = WindowInfo([self windowSize], [self aaLevel]);
    forceResize = YES;
    
    // ---

    [self startIOService];

    sketch->Handler::setIOService(*io);
    sketch->timeline().stepTo(0);
    
    sketch->setup();
    initialized = YES;
}

- (void) resize
{
    Vec2f size = [self windowSize];
    
    if (forceResize || (size != windowInfo.size))
    {
        forceResize = NO;
        windowInfo.size = size;
        
        sketch->resize();
    }
}

- (void) update
{
    sketch->clock().update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME
    [self pollIOService];
    
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

#pragma mark ---------------------------------------- GETTERS ----------------------------------------

- (double) elapsedSeconds
{
    return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-VALUE PER FRAME
}

- (uint32_t) elapsedFrames
{
    return frameCount;
}

- (BOOL) emulated
{
    return SystemManager::getSystemInfo().isSimulator;
}

#pragma mark ---------------------------------------- IO SERVICE ----------------------------------------

- (void) startIOService
{
    io = make_shared<boost::asio::io_service>();
    ioWork = make_shared<boost::asio::io_service::work>(*io);
}

- (void) stopIOService
{
    io->stop();
}

- (void) pollIOService
{
    io->poll();
}

#pragma mark ---------------------------------------- DISPLAY AND WINDOW INFO ----------------------------------------

- (void) updateDisplayInfo
{
    float contentScale = view.contentScaleFactor;
    Vec2i baseSize = [self windowSize] / contentScale;

    // ---
    
    float diagonal = 0;
    int magSize = baseSize.x * baseSize.y;
    
    if (magSize == 320 * 480)
    {
        diagonal = 3.54f; // IPHONE 3GS OR 4
    }
    else if (magSize == 320 * 568)
    {
        diagonal = 4.00f; // IPHONE 5
    }
    else if (magSize == 375 * 667)
    {
        diagonal = 4.70f; // IPHONE 6
    }
    else if (magSize == 360 * 640)
    {
        diagonal = 5.50f; // IPHONE 6+
    }
    else if (magSize == 1024 * 768)
    {
        if (SystemManager::getSystemInfo().isPadMini)
        {
            diagonal = 7.90f; // IPAD MINI
        }
        else
        {
            diagonal = 9.70f; // IPAD
        }
    }
    
    // ---
    
    displayInfo = DisplayInfo::createWithDiagonal(baseSize.x, baseSize.y, diagonal, contentScale);
}

- (Vec2f) windowSize;
{
    Vec2f size;
    
    switch (viewController.interfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
        case UIInterfaceOrientationPortraitUpsideDown:
            size.x = view.frame.size.width;
            size.y = view.frame.size.height;
            break;
            
        case UIInterfaceOrientationLandscapeLeft:
        case UIInterfaceOrientationLandscapeRight:
            size.x = view.frame.size.height;
            size.y = view.frame.size.width;
            break;
    }
    
    return size * view.contentScaleFactor;
}

- (int) aaLevel
{
    switch (view.drawableMultisample)
    {
        case GLKViewDrawableMultisampleNone:
            return 0;
            
        case GLKViewDrawableMultisample4X:
            return 4;
    }
}

#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
    float ax;
    float ay;
    
    switch (viewController.interfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            ax = +acceleration.x;
            ay = +acceleration.y;
            break;
            
        case UIInterfaceOrientationPortraitUpsideDown:
            ax = -acceleration.x;
            ay = -acceleration.y;
            break;
            
        case UIInterfaceOrientationLandscapeLeft:
            ax = +acceleration.y;
            ay = -acceleration.x;
            break;
            
        case UIInterfaceOrientationLandscapeRight:
            ax = -acceleration.y;
            ay = +acceleration.x;
            break;
    }
    
    Vec3f transformed(ax, ay, acceleration.z);
    sketch->accelerated(accelFilter.process(transformed));
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

#pragma mark ---------------------------------------- ACTIONS AND MESSAGES ----------------------------------------

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
