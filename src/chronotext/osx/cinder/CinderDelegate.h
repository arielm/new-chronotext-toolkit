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

- (NSDictionary*) parseJSON:(NSString*)string;
- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;
- (void) sendMessageToSketch:(int)what json:(NSDictionary*)json;
- (void) receiveMessageFromSketch:(int)what body:(NSString*)body;

@end
