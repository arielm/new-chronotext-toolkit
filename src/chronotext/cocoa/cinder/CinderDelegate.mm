#import "CinderDelegate.h"

using namespace std;
using namespace ci;

@implementation CinderDelegate

@synthesize view;
@synthesize sketch;

- (id) initWithCinderApp:(CinderApp*)cinderApp
{
    if (self = [super init])
    {
        view = (NSView*)cinderApp->getWindow()->getNative();
        sketch = cinderApp->sketch;
    }
    
    return self;
}

- (NSDictionary*) parseJSON:(NSString*)string
{
    return [NSJSONSerialization JSONObjectWithData:[string dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nil];
}

- (void) sendMessageToSketch:(int)what json:(NSDictionary*)json
{
    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:0 error:nil];
    NSString *string = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    
    [self sendMessageToSketch:what body:string];
}

- (void) sendMessageToSketch:(int)what
{
    sketch->sendMessage(Message(what));
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    sketch->sendMessage(Message(what, [body UTF8String]));
}

- (void) receiveMessageFromSketch:(int)what body:(NSString*)body
{}

@end
