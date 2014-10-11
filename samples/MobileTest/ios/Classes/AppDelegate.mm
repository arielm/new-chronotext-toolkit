/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "AppDelegate.h"
#import "GLViewController.h"

#include "Sketch.h"

@implementation AppDelegate

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    viewController = [[GLViewController alloc] initWithProperties:[NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithInt:kEAGLRenderingAPIOpenGLES1], kGLViewControllerPropertyRenderingAPI,
        [NSNumber numberWithInt:UIInterfaceOrientationMaskAll], kGLViewControllerPropertyInterfaceOrientationMask,
        [NSNumber numberWithInt:GLKViewDrawableDepthFormatNone], kGLViewControllerPropertyDepthFormat,
        nil]];
    
    viewController.cinderDelegate = self;
    self.sketch = new Sketch(self);
    
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];
    
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
