/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "Application.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    bridge = [[CinderBridge alloc] initWithOptions:launchOptions];
    
    bridge.viewControllerProperties = [NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithInt:kEAGLRenderingAPIOpenGLES1], kGLViewControllerPropertyRenderingAPI,
        [NSNumber numberWithInt:UIInterfaceOrientationMaskLandscape], kGLViewControllerPropertyInterfaceOrientationMask,
        [NSNumber numberWithInt:GLKViewDrawableDepthFormatNone], kGLViewControllerPropertyDepthFormat,
        nil];
    
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];
    
    [window setRootViewController:bridge.viewController];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [bridge release];
    [window release];
    
    [super dealloc];
}

@end
