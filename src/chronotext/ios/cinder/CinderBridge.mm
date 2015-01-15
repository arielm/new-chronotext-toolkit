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

@interface CinderBridge ()
{
    GLKView *view;
    CinderSketch *sketch;
    WindowInfo windowInfo;
    
    BOOL initialized;
    BOOL active;
    BOOL forceResize;

    shared_ptr<boost::asio::io_service> io;
    shared_ptr<boost::asio::io_service::work> ioWork;

    Timer timer;
    uint32_t frameCount;
    
    map<UITouch*, uint32_t> touchIdMap;
}

- (void) performInit;
- (void) performLaunch;

- (void) startIOService;
- (void) stopIOService;
- (void) pollIOService;

- (Vec2f) windowSize;
- (int) aaLevel;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CinderBridge

@synthesize viewController;
@synthesize accelFilter;
@synthesize windowInfo;
@synthesize initialized;
@synthesize active;

- (id) initWithOptions:(NSDictionary*)options
{
    if (self = [super init])
    {
        [self performInit];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidReceiveMemoryWarningNotification) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
    }
    
    return self;
}

- (void) dealloc
{
    sketch->shutdown();
    destroySketch(sketch);
    sketch = nullptr;

    /*
     * TODO:
     *
     * - PROPERLY HANDLE THE SHUTING-DOWN OF "UNDERGOING" TASKS
     * - SEE RELATED TODOS IN Context AND TaskManager
     */
    [self stopIOService];
    INTERN::shutdown();
    
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
        [self performLaunch];
    }
    
    return viewController;
}

- (void) performInit
{
    INTERN::init(system::InitInfo());
    
    sketch = createSketch();
    sketch->setDelegate(self);
    sketch->init();
}

- (void) performLaunch
{
    [self startIOService];
    
    INTERN::launch(system::LaunchInfo(*io));
    sketch->launch();
}

- (void) setup
{
    view = viewController.glView;
    
    windowInfo = WindowInfo([self windowSize], [self aaLevel]);
    forceResize = YES;

    INTERN::setup(system::SetupInfo(windowInfo));

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
    sketch->clock()->update(); // MUST BE CALLED AT THE BEGINNING OF THE FRAME
    
    [self pollIOService];
    
    /*
     * MUST BE CALLED BEFORE Sketch::update
     * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
     *
     * NOTE THAT getTime() COULD HAVE BEEN ALREADY CALLED
     * WITHIN ONE OF THE PREVIOUSLY "POLLED" FUNCTIONS
     */
    double now = sketch->clock()->getTime();
    
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

- (void) startWithReason:(int)reasonId
{
    frameCount = 0;
    
    timer.start();
    sketch->clock()->start();
    
    switch (reasonId)
    {
        case REASON_VIEW_WILL_APPEAR:
        {
            sketch->start(CinderSketch::REASON_APP_SHOWN);
            active = YES;
            break;
        }
            
        case REASON_APPLICATION_DID_BECOME_ACTIVE:
        {
            sketch->start(CinderSketch::REASON_APP_RESUMED);
            break;
        }
    }
}

- (void) stopWithReason:(int)reasonId
{
    timer.stop();
    sketch->clock()->stop();
    
    switch (reasonId)
    {
        case REASON_VIEW_WILL_DISAPPEAR:
        {
            sketch->stop(CinderSketch::REASON_APP_HIDDEN);
            active = NO;
            break;
        }
            
        case REASON_APPLICATION_WILL_RESIGN_ACTIVE:
        {
            sketch->stop(CinderSketch::REASON_APP_PAUSED);
            break;
        }
    }
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
    return getSystemInfo().isSimulator;
}

#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------

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
        sketch->touchesBegan(TouchEvent(WindowRef(), touchList));
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
        sketch->touchesMoved(TouchEvent(WindowRef(), touchList));
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
        sketch->touchesEnded(TouchEvent(WindowRef(), touchList));
    }
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self touchesEnded:touches withEvent:event];
}

#pragma mark ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------

- (void) handleAction:(int)actionId
{}

- (void) handleMessageFromSketch:(int)what body:(NSString*)body
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
        sketch->event(CinderSketch::EVENT_MEMORY_WARNING); // TODO: PASS THROUGH MemoryManager
    }
}

@end
