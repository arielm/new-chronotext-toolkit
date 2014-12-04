/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "CinderDelegate.h"

OBJC_EXPORT NSString* kGLViewControllerPropertyRenderingAPI;
OBJC_EXPORT NSString* kGLViewControllerPropertyPreferredFramesPerSecond;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultipleTouchEnabled;
OBJC_EXPORT NSString* kGLViewControllerPropertyInterfaceOrientationMask;
OBJC_EXPORT NSString* kGLViewControllerPropertyColorFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyDepthFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyStencilFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultisample;

@interface GLViewController : GLKViewController
{
    GLKView *glView;
    CinderDelegate *cinderDelegate;
}

@property (nonatomic, assign) GLKView *glView;
@property (nonatomic, assign) CinderDelegate *cinderDelegate;

- (id) initWithProperties:(NSDictionary*)props;

@end
