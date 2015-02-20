/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
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
        CinderBridge *bridge = nil;
    }
}

@interface CinderBridge ()
{
    BOOL initialized;
    BOOL setup;

    CinderDelegate *cinderDelegate;
    
    map<UITouch*, uint32_t> touchIdMap;
}

- (BOOL) performInit;
- (void) performUninit;

- (Vec2i) windowSize;
- (int) aaLevel;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderBridge

@synthesize listener;
@synthesize viewControllerProperties;

- (id) init
{
    return [self initWithListener:nil];
}

- (id) initWithListener:(NSObject<BridgeListener>*)_listener
{
    if (self = [super init])
    {
        self.listener = _listener;
        [self performInit];
    }
    
    return self;
}

- (void) dealloc
{
    DLOG(@"CinderBridge.dealloc");
    [self performUninit];
    
    [super dealloc];
}

- (BOOL) performInit
{
    if (!initialized)
    {
        DLOG(@"CinderBridge.performInit");

        system::bridge = self;
        cinderDelegate = new CinderDelegate();
        
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center addObserver:self selector:@selector(applicationWillTerminate) name:UIApplicationWillTerminateNotification object:nil];
        [center addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationDidReceiveMemoryWarning) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];

        [self dispatchEvent:SKETCH_WILL_INIT];
        cinderDelegate->performInit();
        [self dispatchEvent:SKETCH_DID_INIT];
        
        // ---
        
        initialized = YES;
    }
    
    return initialized;
}

- (void) performUninit
{
    if (initialized)
    {
        DLOG(@"CinderBridge:performUninit");
        
        if (setup)
        {
            [self dispatchEvent:SKETCH_WILL_SHUTDOWN];
            cinderDelegate->performShutdown();
            [self dispatchEvent:SKETCH_DID_SHUTDOWN];
            
            setup = NO;
        }
        
        if (viewController)
        {
            [viewController release];
            viewController = nil;
        }
        
        // ---
        
        system::bridge = nil;
        
        [self dispatchEvent:SKETCH_WILL_UNINIT];
        cinderDelegate->performUninit();
        [self dispatchEvent:SKETCH_DID_UNINIT];
        
        delete cinderDelegate;
        cinderDelegate = nullptr;
        
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        
        // ---
        
        initialized = NO;
    }
}

- (void) performSetup
{
    if (!setup && initialized)
    {
        DLOG(@"CinderBridge.performSetup");

        [self dispatchEvent:SKETCH_WILL_SETUP];
        cinderDelegate->performSetup(WindowInfo([self windowSize], [self aaLevel]));
        [self dispatchEvent:SKETCH_DID_SETUP];
        
        setup = YES;
    }
}

- (void) performResize
{
    auto size = [self windowSize];
    DLOG(@"CinderBridge:performResize - %dx%d", size.x, size.y);
    
    cinderDelegate->performResize(size);
}

- (void) performUpdate
{
    cinderDelegate->performUpdate();
}

- (void) performDraw
{
    cinderDelegate->performDraw();
}

- (GLViewController*) viewController
{
    if (!viewController)
    {
        viewController = [[GLViewController alloc] initWithBridge:self properties:viewControllerProperties];
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
            
            [self dispatchEvent:VIEW_WILL_APPEAR];
            cinderDelegate->handleEvent(CinderSketch::EVENT_SHOWN);
            
            break;
        }
            
        case REASON_APP_DID_BECOME_ACTIVE:
        {
            DLOG(@"CinderBridge:startWithReason - RESUMED");
            
            [self dispatchEvent:APP_DID_RESUME];
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
            
            [self dispatchEvent:VIEW_WILL_DISAPPEAR];
            cinderDelegate->handleEvent(CinderSketch::EVENT_HIDDEN);
            
            break;
        }
            
        case REASON_APP_WILL_RESIGN_ACTIVE:
        {
            DLOG(@"CinderBridge:stopWithReason - PAUSED");
            
            [self dispatchEvent:APP_WILL_PAUSE];
            cinderDelegate->handleEvent(CinderSketch::EVENT_PAUSED);
            
            break;
        }
    }
}

#pragma mark ---------------------------------------- BRIDGE CALLBACKS ----------------------------------------

- (void) dispatchMessage:(int)what body:(NSString*)body
{
    if (self.listener)
    {
        [listener handleMessage:what body:body];
    }
    else
    {
        [self handleMessage:what body:body];
    }
}

- (void) dispatchEvent:(int)eventId
{
    if (self.listener)
    {
        [listener handleEvent:eventId];
    }
    else
    {
        [self handleEvent:eventId];
    }
}

#pragma mark ---------------------------------------- WINDOW-INFO ----------------------------------------

- (Vec2i) windowSize;
{
    Vec2i size;
    auto view = viewController.view;
    
    if ([UIScreen.mainScreen respondsToSelector:@selector(nativeScale)]) // I.E. IOS 8+
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
    float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+

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
    float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+
    
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
    float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+
    
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
    float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+
    
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
    NSString *body = [NSString stringWithJSONObject:json];
    cinderDelegate->messageFromBridge(what, [body UTF8String]);
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    cinderDelegate->messageFromBridge(what, [body UTF8String]);
}

// ---

- (void) handleMessage:(int)what body:(NSString*)body
{}

- (void) handleEvent:(int)eventId
{}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationWillTerminate
{
    if (initialized)
    {
        DLOG(@"CinderBridge:applicationWillTerminate");
        [self performUninit];
    }
}


- (void) applicationDidReceiveMemoryWarning
{
    if (initialized)
    {
        DLOG(@"CinderBridge:applicationDidReceiveMemoryWarning");
        cinderDelegate->handleEvent(CinderSketch::EVENT_MEMORY_WARNING);
    }
}

- (void) applicationWillResignActive
{
    if (setup && !viewController.appeared)
    {
        DLOG(@"CinderBridge:applicationWillResignActive");
        cinderDelegate->handleEvent(CinderSketch::EVENT_BACKGROUND); // TODO: TEST
    }
}

- (void) applicationDidBecomeActive
{
    if (setup && !viewController.appeared)
    {
        DLOG(@"CinderBridge:applicationDidBecomeActive");
        cinderDelegate->handleEvent(CinderSketch::EVENT_FOREGROUND); // TODO: TEST
    }
}

@end
