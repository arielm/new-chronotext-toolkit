/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import <Cocoa/Cocoa.h>

#import "chronotext/cocoa/utils/NSString+JSON.h"

#include "chronotext/cinder/CinderApp.h"

@interface CinderDelegate : NSObject
{
    NSView *view;
    CinderSketch *sketch;
}

@property (nonatomic, readonly) NSView *view;
@property (nonatomic, readonly) CinderSketch *sketch;

- (id) initWithCinderApp:(CinderApp*)cinderApp;

- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what json:(id)json;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;
- (void) receiveMessageFromSketch:(int)what body:(NSString*)body;

@end
