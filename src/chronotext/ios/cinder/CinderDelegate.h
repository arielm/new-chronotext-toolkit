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

#include "chronotext/cinder/CinderSketch.h"

enum
{
    REASON_VIEW_WILL_APPEAR,
    REASON_VIEW_WILL_DISAPPEAR,
    REASON_APPLICATION_DID_BECOME_ACTIVE,
    REASON_APPLICATION_WILL_RESIGN_ACTIVE
};

@interface CinderDelegate : NSObject <UIAccelerometerDelegate>
{
    GLViewController *viewController;
    chr::CinderSketch *sketch;
    AccelEvent::Filter accelFilter;
}

@property (nonatomic, readonly) GLViewController *viewController;
@property (nonatomic, readonly) chr::CinderSketch *sketch;
@property (nonatomic, assign) AccelEvent::Filter accelFilter;
@property (nonatomic, readonly) const chr::WindowInfo &windowInfo;
@property (nonatomic, readonly) BOOL initialized;
@property (nonatomic, readonly) BOOL active;
@property (nonatomic, readonly) double elapsedSeconds;
@property (nonatomic, readonly) uint32_t elapsedFrames;
@property (nonatomic, readonly) BOOL emulated;

- (id) initWithOptions:(NSDictionary*)options;

- (GLViewController*) bindWithViewController:(GLViewController*)controller;

- (void) setup;
- (void) resize;
- (void) update;
- (void) draw;

- (void) startWithReason:(int)reasonId;
- (void) stopWithReason:(int)reasonId;

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

- (void) handleAction:(int)actionId;
- (void) handleMessageFromSketch:(int)what body:(NSString*)body;
- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what json:(id)json;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;

@end
