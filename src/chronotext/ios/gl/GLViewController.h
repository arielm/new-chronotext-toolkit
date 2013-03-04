#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "CinderDelegate.h"

OBJC_EXPORT NSString* kGLViewControllerPropertyPreferredFramesPerSecond;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultipleTouchEnabled;
OBJC_EXPORT NSString* kGLViewControllerPropertyInterfaceOrientation;

OBJC_EXPORT NSString* kGLViewControllerPropertyColorFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyDepthFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyStencilFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultisample;

@interface GLViewController : GLKViewController
{
    NSMutableDictionary *properties;
    int interfaceOrientation;

    GLKView *glView;
    CinderDelegate *cinderDelegate;
    
    BOOL started;
    BOOL stopped;
    
	int ticks;
	NSTimeInterval t0;
	NSTimeInterval elapsed;
}

@property (nonatomic, assign) GLKView *glView;
@property (nonatomic, assign) CinderDelegate *cinderDelegate;

- (id) initWithProperties:(NSDictionary*)props;

@end
