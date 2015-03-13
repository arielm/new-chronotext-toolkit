/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "chronotext/osx/cinder/OverlayWindow.h"
#import "chronotext/osx/cinder/OverlayView.h"

@implementation OverlayWindow

/*
 * REFERENCES:
 * https://forum.libcinder.org/topic/placing-an-nsview-above-the-cinderview
 * http://stackoverflow.com/questions/15753313/resizing-an-nswindow-setup-as-a-childwindow
 * http://stackoverflow.com/questions/4946342/why-nswindow-without-stylemasknstitledwindowmask-can-not-be-keywindow
 */

- (id) initWithParentWindow:(NSWindow*)window
{
    if (self = [super initWithContentRect:[window convertRectToScreen:[window.contentView bounds]] styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO])
    {
        [self setContentView:[[OverlayView new] autorelease]]; // IT'S NECESSARY TO REPLACE THE DEFAULT NSView, IN ORDER TO FLIP VERTICAL COORDINATES

        self.backgroundColor = [NSColor clearColor];
        self.opaque = NO;
        self.autorecalculatesKeyViewLoop = YES;;
        
        [window addChildWindow:self ordered:NSWindowAbove];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];
    }
    
    return self;
}

- (void) dealloc
{
    [super dealloc];
}

- (BOOL) canBecomeKeyWindow
{
    [self.parentWindow becomeKeyWindow]; // TRICK TO AVOID THE CinderWindow TO APPEAR UNFOCUSED WHEN INTERACTING WITH NSViews ON THE OverlayWindow
    return YES;
}

- (void) windowWillClose:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self close]; // WILL RELEASE THE NSWindow (AND ALL THE NSViews INSIDE)
}

@end
