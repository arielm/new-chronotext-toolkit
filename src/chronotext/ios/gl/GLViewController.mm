/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "chronotext/ios/gl/GLViewController.h"
#import "chronotext/ios/cinder/CinderBridge.h"

NSString* kGLViewControllerPropertyRenderingAPI = @"kGLViewControllerPropertyRenderingAPI";
NSString* kGLViewControllerPropertyPreferredFramesPerSecond = @"kGLViewControllerPropertyPreferredFramesPerSecond";
NSString* kGLViewControllerPropertyMultipleTouchEnabled = @"kGLViewControllerPropertyMultipleTouchEnabled";
NSString* kGLViewControllerPropertyInterfaceOrientationMask = @"kGLViewControllerPropertyInterfaceOrientationMask";
NSString* kGLViewControllerPropertyColorFormat = @"kGLViewControllerPropertyColorFormat";
NSString* kGLViewControllerPropertyDepthFormat = @"kGLViewControllerPropertyDepthFormat";
NSString* kGLViewControllerPropertyStencilFormat = @"kGLViewControllerPropertyStencilFormat";
NSString* kGLViewControllerPropertyMultisample = @"kGLViewControllerPropertyMultisample";

@interface GLViewController ()
{
    NSMutableDictionary *properties;
    int interfaceOrientationMask;
    
    BOOL setupRequest;
    
    BOOL resizeRequest;
    int viewportWidth;
    int viewportHeight;

    BOOL started;
    BOOL startRequest;
    int startReason;
}

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

@end

@implementation GLViewController

@synthesize cinderBridge;
@synthesize glView;

- (id) initWithBridge:(CinderBridge*)bridge properties:(NSDictionary*)_properties
{
    if (self = [super init])
    {
        cinderBridge = bridge;

        NSDictionary *defaults = [NSDictionary dictionaryWithObjectsAndKeys:
            [NSNumber numberWithInt:kEAGLRenderingAPIOpenGLES1], kGLViewControllerPropertyRenderingAPI,
            [NSNumber numberWithInt:60], kGLViewControllerPropertyPreferredFramesPerSecond,
            [NSNumber numberWithBool:YES], kGLViewControllerPropertyMultipleTouchEnabled,
            [NSNumber numberWithInt:UIInterfaceOrientationMaskPortrait], kGLViewControllerPropertyInterfaceOrientationMask,
            [NSNumber numberWithInt:GLKViewDrawableColorFormatRGBA8888], kGLViewControllerPropertyColorFormat,
            [NSNumber numberWithInt:GLKViewDrawableDepthFormat24], kGLViewControllerPropertyDepthFormat,
            [NSNumber numberWithInt:GLKViewDrawableStencilFormatNone], kGLViewControllerPropertyStencilFormat,
            [NSNumber numberWithInt:GLKViewDrawableMultisampleNone], kGLViewControllerPropertyMultisample,
            nil];
        
        if (_properties)
        {
            properties = [[NSMutableDictionary alloc] initWithDictionary:_properties];
        }
        else
        {
            properties = [[NSMutableDictionary alloc] init];
        }
        
        for (id key in defaults)
        {
            if (![properties objectForKey:key])
            {
                [properties setObject:[defaults objectForKey:key] forKey:key];
            }
        }

        interfaceOrientationMask = [[properties objectForKey:kGLViewControllerPropertyInterfaceOrientationMask] intValue];
    }
    
    return self;
}

- (void) dealloc
{
    [properties release];
    [super dealloc];
}

- (void) loadView
{
    [super loadView];

    glView = (GLKView*)self.view;
    glView.context = [[[EAGLContext alloc] initWithAPI:(EAGLRenderingAPI)[[properties objectForKey:kGLViewControllerPropertyRenderingAPI] intValue]] autorelease];
    
    self.preferredFramesPerSecond = [[properties objectForKey:kGLViewControllerPropertyPreferredFramesPerSecond] intValue];
    self.view.multipleTouchEnabled = [[properties objectForKey:kGLViewControllerPropertyMultipleTouchEnabled] boolValue];
    
    glView.drawableColorFormat = (GLKViewDrawableColorFormat)[[properties objectForKey:kGLViewControllerPropertyColorFormat] intValue];
    glView.drawableDepthFormat = (GLKViewDrawableDepthFormat)[[properties objectForKey:kGLViewControllerPropertyDepthFormat] intValue];
    glView.drawableStencilFormat = (GLKViewDrawableStencilFormat)[[properties objectForKey:kGLViewControllerPropertyStencilFormat] intValue];
    glView.drawableMultisample = (GLKViewDrawableMultisample)[[properties objectForKey:kGLViewControllerPropertyMultisample] intValue];
    
    // ---
    
    [EAGLContext setCurrentContext:glView.context]; // MUST TAKE PLACE BEFORE "SETUP"
    
    [cinderBridge setup];
    resizeRequest = YES;
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (self.view)
    {
#ifdef DEBUG
        NSLog(@"GLViewController - viewWillAppear");
#endif
        
        [self startWithReason:REASON_VIEW_WILL_APPEAR];
        
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
    }
}

- (void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    if (self.view)
    {
#ifdef DEBUG
        NSLog(@"GLViewController - viewWillDisappear");
#endif
        
        [self stopWithReason:REASON_VIEW_WILL_DISAPPEAR];
        
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
        [center removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    }
}

- (void) startWithReason:(int)reason
{
    if (!started)
    {
        [EAGLContext setCurrentContext:glView.context]; // MUST TAKE PLACE BEFORE "START" AND "DRAW"
        
        started = YES;
        startRequest = YES;
        startReason = reason;
    }
}

- (void) stopWithReason:(int)reason
{
    if (started)
    {
        [cinderBridge stopWithReason:reason];
        started = NO;
    }
}

- (void) update
{
    if (started)
    {
        if (resizeRequest)
        {
            resizeRequest = NO;
            [cinderBridge resize];
        }
        
        if (startRequest)
        {
            [cinderBridge startWithReason:startReason];
        }
        
        [cinderBridge update];
    }
    
    startRequest = NO;
}

- (void) glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    if (started)
    {
        [cinderBridge draw];
    }
}

/*
 * FOR iOS 7+
 */
- (BOOL) prefersStatusBarHidden
{
    return YES;
}

#pragma mark ---------------------------------------- ORIENTATION ----------------------------------------

- (void) willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    /*
     * WHEN ROTATING FROM PORTRAIT TO LANDSCAPE (OR VICE-VERSA):
     *
     * - THIS IS NECESSARY, OTHERWISE SCREEN WILL BE DEFORMED
     *
     * - CURRENTLY, THERE IS STILL A SHORT "STRETCH ARTEFACT" DURING ROTATION:
     *   A MINOR ISSUE WHICH WON'T AFFECT MOST OF THE APPS, WHICH ARE
     *   SUPPOSED TO BE EITHER LANDSCAPE RIGHT/LEFT, OR PORTAIT BOTTOM/UP
     */
    resizeRequest = YES;
}

/*
 * IMPORTANT:
 *
 * - SUPPORTED ORIENTATIONS SHOULD ALSO BE LISTED IN Info.plist
 * - THE APP WILL START USING THE 1ST ORIENTATION IN THE LIST (Item 0)
 */

/*
 * FOR iOS 6+
 */

- (BOOL) shouldAutorotate
{
    return YES;
}

- (NSUInteger) supportedInterfaceOrientations
{
    return interfaceOrientationMask;
}

/*
 * FOR iOS 5.x
 */

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return (1 << toInterfaceOrientation) & interfaceOrientationMask;
}

#pragma mark ---------------------------------------- TOUCH ----------------------------------------

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    [cinderBridge touchesBegan:touches withEvent:event];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    [cinderBridge touchesMoved:touches withEvent:event];
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    [cinderBridge touchesEnded:touches withEvent:event];
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    [cinderBridge touchesCancelled:touches withEvent:event];
}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationDidBecomeActive
{
    if (self.view)
    {
#ifdef DEBUG
        NSLog(@"GLViewController - applicationDidBecomeActive");
#endif
        
        [self startWithReason:REASON_APPLICATION_DID_BECOME_ACTIVE];
    }
}

- (void) applicationWillResignActive
{
    if (self.view)
    {
#ifdef DEBUG
        NSLog(@"GLViewController - applicationWillResignActive");
#endif
        
        [self stopWithReason:REASON_APPLICATION_WILL_RESIGN_ACTIVE];
    }
}

@end
