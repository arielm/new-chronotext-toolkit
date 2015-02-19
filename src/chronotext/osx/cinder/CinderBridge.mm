/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "chronotext/osx/cinder/CinderBridge.h"
#import "chronotext/cocoa/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace chr;

@implementation CinderBridge

@synthesize cinderDelegate;
@synthesize view;

- (id) initWithDelegate:(CinderDelegate*)delegate
{
    if (self = [super init])
    {
        cinderDelegate = delegate;
        view = reinterpret_cast<NSView*>(delegate->getWindow()->getNative());
    }
    
    return self;
}

#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

- (void) sendMessageToSketch:(int)what
{
    cinderDelegate->messageFromBridge(what);
}

- (void) sendMessageToSketch:(int)what json:(id)json
{
    NSString *body = [NSString stringWithJSONObject:json];
    cinderDelegate->messageFromBridge(what, [body UTF8String]);
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
    cinderDelegate->messageFromBridge(what, [body UTF8String]);
}

- (void) handleMessage:(int)what body:(NSString*)body
{}

@end
