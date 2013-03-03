#import "GLViewController.h"


@interface GLViewController ()

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

@end

@implementation GLViewController

@synthesize glView;
@synthesize cinderDelegate;

- (id) init
{
    if (self = [super init])
    {
        interfaceOrientation = UIInterfaceOrientationPortrait; // FIXME: SHOULD BE DEFINED EXTERNALLY
    }
    
    return self;
}

- (void) loadView
{
    [super loadView];

    glView = (GLKView*)self.view;
    glView.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];
    
    /*
     * FIXME: DEFINE DEPTH-BUFFER, AA, FPS, MULTI-TOUCH EXTERNALLY
     */
    self.preferredFramesPerSecond = 60;
    self.view.multipleTouchEnabled = YES;
    
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
