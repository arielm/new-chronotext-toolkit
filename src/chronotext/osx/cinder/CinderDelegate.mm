/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace chr;

@implementation CinderDelegate

@synthesize view;
@synthesize sketch;

- (id) initWithCinderApp:(CinderApp*)cinderApp
{
    if (self = [super init])
    {
        view = (NSView*)cinderApp->getWindow()->getNative();
        sketch = cinderApp->getSketch();
    }
    
    return self;
}

- (void) sendMessageToSketch:(int)what
{
    sketch->sendMessage(Message(what));
}

- (void) sendMessageToSketch:(int)what json:(id)json
{
    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:0 error:nil];
    NSString *string = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    
    [self sendMessageToSketch:what body:string];
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    sketch->sendMessage(Message(what, [body UTF8String]));
}

- (void) receiveMessageFromSketch:(int)what body:(NSString*)body
{}

@end
