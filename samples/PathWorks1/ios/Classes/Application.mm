/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "Application.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    bridge = [[CinderBridge alloc] init];
    
    bridge.viewControllerProperties =
    @{
      GLViewControllerRenderingAPIKey: @(kEAGLRenderingAPIOpenGLES1),
      GLViewControllerInterfaceOrientationMaskKey: @(UIInterfaceOrientationMaskLandscape),
      GLViewControllerDepthFormatKey: @(GLKViewDrawableDepthFormatNone),
      };
    
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];
    
    [window setRootViewController:bridge.viewController];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [window release];
    [bridge release];
    
    [super dealloc];
}

@end
