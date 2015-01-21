/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * "TOUCH MAPPING" BASED ON CINDER:
 * https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/app/CinderViewCocoaTouch.mm
 */

#import "chronotext/ios/cinder/CinderBridge.h"
#import "chronotext/cocoa/utils/Utils.h"

#include "chronotext/Context.h"

#include <boost/asio.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

namespace chr
{
    namespace system
    {
        CinderBridge *bridge = nullptr;
    }
}

@interface CinderBridge ()
{
    CinderDelegate *cinderDelegate;

    BOOL initialized;
    BOOL setup;
    BOOL launched;
    
    map<UITouch*, uint32_t> touchIdMap;
}

- (BOOL) performInit;
- (void) performLaunch;
- (void) performShutdown;

- (Vec2i) windowSize;
- (int) aaLevel;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderBridge

@synthesize viewControllerProperties;

- (id) init
{
    if (self = [super init])
    {
        [self performInit];
    }
    
    return self;
}

- (void) dealloc
{
    DLOG(@"CinderBridge.dealloc");
    [self performShutdown];
    
    [super dealloc];
}

- (BOOL) performInit
{
    if (!initialized)
    {
        DLOG(@"CinderBridge.performInit");

        system::bridge = self;

        cinderDelegate = new CinderDelegate();
        cinderDelegate->init();
        
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center addObserver:self selector:@selector(applicationWillTerminate) name:UIApplicationWillTerminateNotification object:nil];
        [center addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationDidReceiveMemoryWarning) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];

        // ---
        
        initialized = YES;
    }
    
    return initialized;
}

- (void) performLaunch
{
    if (!launched)
    {
        DLOG(@"CinderBridge.performLaunch");

        viewController = [[GLViewController alloc] initWithBridge:self properties:viewControllerProperties];

        cinderDelegate->launch();
        
        // ---
        
        launched = YES;
    }
}

- (void) performSetup
{
    if (!setup)
    {
        auto size = [self windowSize];
        DLOG(@"CinderBridge:performSetup - %dx%d", size.x, size.y);

        cinderDelegate->setup(WindowInfo(size, [self aaLevel]));
        
        // ---
        
        setup = YES;
    }
}

- (void) performShutdown
{
    if (initialized)
    {
        DLOG(@"CinderBridge:performShutdown");
        
        if (launched)
        {
            [viewController release];
            viewController = nil;
        }
        
        system::bridge = nil;
        
        cinderDelegate->shutdown();
        delete cinderDelegate;
        cinderDelegate = nullptr;
        
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        
        // ---
        
        initialized = false;
        launched = false;
        setup = false;
    }
}

- (void) performResize
{
    auto size = [self windowSize];
    DLOG(@"CinderBridge:performResize - %dx%d", size.x, size.y);
    
    cinderDelegate->resize(size);
}

- (void) performUpdate
{
    cinderDelegate->update();
}

- (void) performDraw
{
    cinderDelegate->draw();
}

- (GLViewController*) viewController
{
    if (!viewController)
    {
        [self performLaunch];
    }
    
    return viewController;
}

- (void) startWithReason:(int)reason
{
    switch (reason)
    {
        case REASON_VIEW_WILL_APPEAR:
        {
            DLOG(@"CinderBridge:startWithReason - SHOWN");
            cinderDelegate->handleEvent(CinderSketch::EVENT_SHOWN);
            break;
        }
            
        case REASON_APPLICATION_DID_BECOME_ACTIVE:
        {
            DLOG(@"CinderBridge:startWithReason - RESUMED");
            cinderDelegate->handleEvent(CinderSketch::EVENT_RESUMED);
            break;
        }
    }
}

- (void) stopWithReason:(int)reason
{
    switch (reason)
    {
        case REASON_VIEW_WILL_DISAPPEAR:
        {
            DLOG(@"CinderBridge:stopWithReason - HIDDEN");
            cinderDelegate->handleEvent(CinderSketch::EVENT_HIDDEN);
            break;
        }
            
        case REASON_APPLICATION_WILL_RESIGN_ACTIVE:
        {
            DLOG(@"CinderBridge:stopWithReason - PAUSED");
            cinderDelegate->handleEvent(CinderSketch::EVENT_PAUSED);
            break;
        }
    }
}

#pragma mark ---------------------------------------- WINDOW-INFO ----------------------------------------

/*
 * FIXME: SCREEN-SIZE EVALUATION ON IPHONE 6 IS MORE COMPLICATED...
 * https://developer.apple.com/library/ios/releasenotes/General/WhatsNewIniOS/Articles/iOS8.html#//apple_ref/doc/uid/TP40014205-SW46
 */

- (Vec2i) windowSize;
{
    Vec2i size;
    auto view = viewController.view;
    
    if (getSystemInfo().osVersion[0] >= 8)
    {
        size.x = view.frame.size.width;
        size.y = view.frame.size.height;
    }
    else
    {
        switch (viewController.interfaceOrientation)
        {
            case UIDeviceOrientationUnknown:
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
    }
    
    return size * view.contentScaleFactor;
}

- (int) aaLevel
{
    switch (viewController.glView.drawableMultisample)
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
        case UIDeviceOrientationUnknown:
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
    
    cinderDelegate->handleAcceleration(Vec3f(ax, ay, acceleration.z));
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
        
        for (auto &element : touchIdMap)
        {
            if (element.second == candidateId)
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
    auto view = viewController.view;
    float scale = view.contentScaleFactor; // XXX

    vector<TouchEvent::Touch> activeTouches;
    
    for (auto &element : touchIdMap)
    {
        auto &touch = element.first;
        auto &touchId = element.second;
        
        CGPoint pt = [touch locationInView:view];
        CGPoint prevPt = [touch previousLocationInView:view];
        activeTouches.emplace_back(Vec2f(pt.x, pt.y) * scale, Vec2f(prevPt.x, prevPt.y) * scale, touchId, [touch timestamp], touch);
    }
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    auto view = viewController.view;
    float scale = view.contentScaleFactor; // XXX
    
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
        cinderDelegate->touchesBegan(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    auto view = viewController.view;
    float scale = view.contentScaleFactor; // XXX
    
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
        cinderDelegate->touchesMoved(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    auto view = viewController.view;
    float scale = view.contentScaleFactor; // XXX
    
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
        cinderDelegate->touchesEnded(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self touchesEnded:touches withEvent:event];
}

#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

- (void) sendMessageToSketch:(int)what
{
    cinderDelegate->messageFromBridge(what);
}

- (void) sendMessageToSketch:(int)what json:(id)json
{
    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:0 error:nil];
    NSString *string = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    
    cinderDelegate->messageFromBridge(what, [string UTF8String]);
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    cinderDelegate->messageFromBridge(what, [body UTF8String]);
}

- (void) handleMessage:(int)what body:(NSString*)body
{}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationWillTerminate
{
    if (initialized)
    {
        DLOG(@"CinderBridge:applicationWillTerminate");
        [self performShutdown];
    }
}


- (void) applicationDidReceiveMemoryWarning
{
    if (initialized)
    {
        DLOG(@"CinderBridge:applicationWillTerminate");
        cinderDelegate->handleEvent(CinderSketch::EVENT_MEMORY_WARNING);
    }
}

- (void) applicationWillResignActive
{
    if (setup && !viewController.appeared)
    {
        DLOG(@"CinderBridge:applicationWillResignActive");
        cinderDelegate->handleEvent(CinderSketch::EVENT_BACKGROUND);
    }
}

- (void) applicationDidBecomeActive
{
    if (setup && !viewController.appeared)
    {
        DLOG(@"CinderBridge:applicationDidBecomeActive");
        cinderDelegate->handleEvent(CinderSketch::EVENT_FOREGROUND);
    }
}

@end
