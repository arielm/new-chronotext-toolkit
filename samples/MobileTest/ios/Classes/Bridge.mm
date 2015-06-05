/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "Bridge.h"

@implementation Bridge

- (id) init
{
    if (self = [super init])
    {
        self.viewControllerProperties =
        @{
          GLViewControllerRenderingAPIKey: @(kEAGLRenderingAPIOpenGLES1),
          GLViewControllerInterfaceOrientationMaskKey: @(UIInterfaceOrientationMaskAll),
          GLViewControllerDepthFormatKey: @(GLKViewDrawableDepthFormatNone),
          };

        window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
        window.backgroundColor = [UIColor blackColor];

        [window setRootViewController:self.viewController];
        [window makeKeyAndVisible];
    }
    
    return self;
}

- (void) dealloc
{
    [window release];
    [super dealloc];
}

@end
