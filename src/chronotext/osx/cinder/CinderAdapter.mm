/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "CinderAdapter.h"

using namespace std;
using namespace ci;
using namespace chr;

@implementation CinderAdapter

@synthesize sketch;
@synthesize view;

- (id) initWithCinderDelegate:(CinderDelegate*)delegate
{
    if (self = [super init])
    {
        sketch = delegate->getSketch();
        assert(sketch);
        
        assert(delegate->getWindow());
        view = reinterpret_cast<NSView*>(delegate->getWindow()->getNative());
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
