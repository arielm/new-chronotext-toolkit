#import <Cocoa/Cocoa.h>

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
