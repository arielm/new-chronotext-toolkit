#import "GLViewController.h"

NSString* kGLViewControllerPropertyPreferredFramesPerSecond = @"kGLViewControllerPropertyPreferredFramesPerSecond";
NSString* kGLViewControllerPropertyMultipleTouchEnabled = @"kGLViewControllerPropertyMultipleTouchEnabled";
NSString* kGLViewControllerPropertyInterfaceOrientation = @"kGLViewControllerPropertyInterfaceOrientation";

NSString* kGLViewControllerPropertyColorFormat = @"kGLViewControllerPropertyColorFormat";
NSString* kGLViewControllerPropertyDepthFormat = @"kGLViewControllerPropertyDepthFormat";
NSString* kGLViewControllerPropertyStencilFormat = @"kGLViewControllerPropertyStencilFormat";
NSString* kGLViewControllerPropertyMultisample = @"kGLViewControllerPropertyMultisample";

@interface GLViewController ()

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

@end

@implementation GLViewController

@synthesize glView;
@synthesize cinderDelegate;

- (id) initWithProperties:(NSDictionary*)props
{
    if (self = [super init])
    {
        NSDictionary *defaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInt:60], kGLViewControllerPropertyPreferredFramesPerSecond,
                                  [NSNumber numberWithBool:NO], kGLViewControllerPropertyMultipleTouchEnabled,
                                  [NSNumber numberWithInt:UIInterfaceOrientationPortrait], kGLViewControllerPropertyInterfaceOrientation,
                                  [NSNumber numberWithInt:GLKViewDrawableColorFormatRGBA8888], kGLViewControllerPropertyColorFormat,
                                  [NSNumber numberWithInt:GLKViewDrawableDepthFormat24], kGLViewControllerPropertyDepthFormat,
                                  [NSNumber numberWithInt:GLKViewDrawableStencilFormatNone], kGLViewControllerPropertyStencilFormat,
                                  [NSNumber numberWithInt:GLKViewDrawableMultisampleNone], kGLViewControllerPropertyMultisample,
                                  nil];
        
        if (props)
        {
            properties = [[NSMutableDictionary alloc] initWithDictionary:props];
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
        
        // ---
        
        interfaceOrientation = [[properties objectForKey:kGLViewControllerPropertyInterfaceOrientation] intValue];
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
    glView.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];
    
    self.preferredFramesPerSecond = [[properties objectForKey:kGLViewControllerPropertyPreferredFramesPerSecond] intValue];
    self.view.multipleTouchEnabled = [[properties objectForKey:kGLViewControllerPropertyMultipleTouchEnabled] boolValue];
    
    glView.drawableColorFormat = (GLKViewDrawableColorFormat)[[properties objectForKey:kGLViewControllerPropertyColorFormat] intValue];
    glView.drawableDepthFormat = (GLKViewDrawableDepthFormat)[[properties objectForKey:kGLViewControllerPropertyDepthFormat] intValue];
    glView.drawableStencilFormat = (GLKViewDrawableStencilFormat)[[properties objectForKey:kGLViewControllerPropertyStencilFormat] intValue];
    glView.drawableMultisample = (GLKViewDrawableMultisample)[[properties objectForKey:kGLViewControllerPropertyMultisample] intValue];
    
    // ---
    
    cinderDelegate.view = glView;
	cinderDelegate.viewController = self;

    /*
     * NECESSARY BEFORE SETUP
     */
    [EAGLContext setCurrentContext:glView.context];

	[cinderDelegate setup];
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (self.view)
    {
        NSLog(@"EAGLViewController - viewWillAppear");
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
        NSLog(@"EAGLViewController - viewWillDisappear");
        [self stopWithReason:REASON_VIEW_WILL_DISAPEAR];
        
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
        [center removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    }
}

- (void) startWithReason:(int)reason
{
    if (!started)
    {
        ticks = 0;
        
        /*
         * THIS *MUST* TAKE PLACE BEFORE DRAWING
         */
        [cinderDelegate startWithReason:reason];
        
        started = YES;
        stopped = NO;
    }
}

- (void) stopWithReason:(int)reason
{
    if (started)
    {
        NSLog(@"AVERAGE FRAME-RATE: %f FRAMES PER SECOND", ticks / elapsed);
        
        [cinderDelegate stopWithReason:reason];
        
        started = NO;
        stopped = YES;
    }
}

- (void) update
{
    NSTimeInterval now = [NSDate timeIntervalSinceReferenceDate];
    
    if (ticks == 0)
    {
		t0 = now;
    }
	
    ticks++;
    elapsed = now - t0;
    
    // ---
    
    [cinderDelegate update];
}

- (void) glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    [cinderDelegate draw];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
	return (toInterfaceOrientation == interfaceOrientation);
}

- (NSUInteger) supportedInterfaceOrientations
{
    return interfaceOrientation;
}

- (BOOL) shouldAutorotate
{
    return NO;
}

#pragma mark ---------------------------------------- TOUCH ----------------------------------------

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[cinderDelegate touchesBegan:touches withEvent:event];
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[cinderDelegate touchesMoved:touches withEvent:event];
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[cinderDelegate touchesEnded:touches withEvent:event];
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
	[cinderDelegate touchesCancelled:touches withEvent:event];
}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationDidBecomeActive
{
    if (self.glView)
    {
        NSLog(@"EAGLViewController - applicationDidBecomeActive");
        [self startWithReason:REASON_APPLICATION_DID_BECOME_ACTIVE];
    }
}

- (void) applicationWillResignActive
{
    if (self.glView)
    {
        NSLog(@"EAGLViewController - applicationWillResignActive");
        [self stopWithReason:REASON_APPLICATION_WILL_RESIGN_ACTIVE];
    }
}

@end
