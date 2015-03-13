/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import <GLKit/GLKit.h>

OBJC_EXPORT NSString* kGLViewControllerPropertyRenderingAPI;
OBJC_EXPORT NSString* kGLViewControllerPropertyPreferredFramesPerSecond;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultipleTouchEnabled;
OBJC_EXPORT NSString* kGLViewControllerPropertyInterfaceOrientationMask;
OBJC_EXPORT NSString* kGLViewControllerPropertyColorFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyDepthFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyStencilFormat;
OBJC_EXPORT NSString* kGLViewControllerPropertyMultisample;

@class CinderBridge;

enum
{
    REASON_VIEW_WILL_APPEAR,
    REASON_VIEW_WILL_DISAPPEAR,
    REASON_APP_DID_BECOME_ACTIVE,
    REASON_APP_WILL_RESIGN_ACTIVE,
};

@interface GLViewController : GLKViewController
{
    CinderBridge *cinderBridge;
    GLKView *glView;
}

@property (nonatomic, readonly) CinderBridge *cinderBridge;
@property (nonatomic, readonly) GLKView *glView;
@property (nonatomic, readonly) BOOL appeared;

- (id) initWithBridge:(CinderBridge*)bridge properties:(NSDictionary*)properties;

@end
