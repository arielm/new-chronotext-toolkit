/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "chronotext/cocoa/utils/NSString+JSON.h"

#include "chronotext/desktop/cinder/CinderDelegate.h"

@interface CinderBridge : NSObject
{
    chr::CinderDelegate *cinderDelegate;
    NSView *view;
}

@property (nonatomic, readonly) chr::CinderDelegate *cinderDelegate;
@property (nonatomic, readonly) NSView *view;

/*
 * WARNING: WILL CRASH IF INVOKED BEFORE AppNative::setup()
 */
- (id) initWithDelegate:(chr::CinderDelegate*)delegate;

- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what json:(id)json;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;

- (void) handleMessage:(int)what body:(NSString*)body;

@end
