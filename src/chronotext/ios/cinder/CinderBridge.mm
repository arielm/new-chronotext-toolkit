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

#include "chronotext/Context.h"

#include <boost/asio.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

namespace chr
{
    namespace INTERN
    {
        CinderBridge *bridge = nullptr;
    }
}

@interface CinderBridge ()
{
    GLKView *view;
    CinderDelegate *cinderDelegate;
    
    BOOL initialized;
    BOOL active;
    
    map<UITouch*, uint32_t> touchIdMap;
}

- (Vec2f) windowSize;
- (int) aaLevel;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderBridge

@synthesize viewController;

- (id) initWithOptions:(NSDictionary*)options
{
    if (self = [super init])
    {
        INTERN::bridge = self;
        
        cinderDelegate = new CinderDelegate();
        cinderDelegate->init();
        
        // ---
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidReceiveMemoryWarningNotification) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
    }
    
    return self;
}

- (void) dealloc
{
    cinderDelegate->shutdown();
    delete cinderDelegate;
    
    INTERN::bridge = nullptr;
    
    // ---
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];

    [viewController release];
    [super dealloc];
}

- (GLViewController*) createViewControllerWithProperties:(NSDictionary*)properties
{
    if (!viewController)
    {
        viewController = [[GLViewController alloc] initWithBridge:self properties:properties];
        cinderDelegate->launch();
    }
    
    return viewController;
}

- (void) setup
{
    view = viewController.glView;
    
    cinderDelegate->setup(WindowInfo([self windowSize], [self aaLevel]));
    
    initialized = YES;
}

- (void) resize
{
    cinderDelegate->resize([self windowSize]);
}

- (void) update
{
    cinderDelegate->update();
}

- (void) draw
{
    cinderDelegate->draw();
}

- (void) startWithReason:(int)reason
{
    switch (reason)
    {
        case REASON_VIEW_WILL_APPEAR:
        {
            cinderDelegate->start(CinderSketch::REASON_APP_SHOWN); // TODO: USE CinderDelegate::handleEvent() INSTEAD?
            active = YES;
            break;
        }
            
        case REASON_APPLICATION_DID_BECOME_ACTIVE:
        {
            cinderDelegate->start(CinderSketch::REASON_APP_RESUMED); // TODO: USE CinderDelegate::handleEvent() INSTEAD?
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
            cinderDelegate->stop(CinderSketch::REASON_APP_HIDDEN); // TODO: USE CinderDelegate::handleEvent() INSTEAD?
            active = NO;
            break;
        }
            
        case REASON_APPLICATION_WILL_RESIGN_ACTIVE:
        {
            cinderDelegate->stop(CinderSketch::REASON_APP_PAUSED); // TODO: USE CinderDelegate::handleEvent() INSTEAD?
            break;
        }
    }
}

#pragma mark ---------------------------------------- WINDOW-INFO ----------------------------------------

/*
 * FIXME: SCREEN-SIZE EVALUATION ON IPHONE 6 IS MORE COMPLICATED...
 * https://developer.apple.com/library/ios/releasenotes/General/WhatsNewIniOS/Articles/iOS8.html#//apple_ref/doc/uid/TP40014205-SW46
 */

- (Vec2f) windowSize;
{
    Vec2f size;
    
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

- (void) applicationWillResignActive
{
    if (initialized && !active)
    {
        cinderDelegate->handleEvent(CinderSketch::EVENT_BACKGROUND);
    }
}

- (void) applicationDidBecomeActive
{
    if (initialized && !active)
    {
        cinderDelegate->handleEvent(CinderSketch::EVENT_FOREGROUND);
    }
}

- (void) applicationDidReceiveMemoryWarningNotification
{
    if (initialized)
    {
        cinderDelegate->handleEvent(CinderSketch::EVENT_MEMORY_WARNING);
    }
}

@end
