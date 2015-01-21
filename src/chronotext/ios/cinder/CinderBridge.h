/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * "TOUCH MAPPING" BASED ON CINDER:
 * https://github.com/cinder/Cinder/blob/v0.8.5/include/cinder/app/CinderViewCocoaTouch.h
 */

#import "chronotext/cocoa/utils/NSString+JSON.h"
#import "chronotext/ios/gl/GLViewController.h"

@interface CinderBridge : NSObject <UIAccelerometerDelegate>
{
    GLViewController *viewController;
    NSDictionary *viewControllerProperties;
}

@property (nonatomic, readonly) GLViewController *viewController;
@property (nonatomic, retain) NSDictionary *viewControllerProperties;

- (void) performSetup;
- (void) performResize;
- (void) performUpdate;
- (void) performDraw;

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what json:(id)json;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;

- (void) handleMessage:(int)what body:(NSString*)body;

@end

namespace chr
{
    namespace system
    {
        extern CinderBridge *bridge;
    }
}
