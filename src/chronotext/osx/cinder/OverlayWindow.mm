/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "OverlayWindow.h"

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
        self.backgroundColor = [NSColor clearColor];
        [self setOpaque:NO];
        [self setAutorecalculatesKeyViewLoop:YES];
        
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
    [self.parentWindow becomeKeyWindow];
    return YES;
}

- (void) windowWillClose:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self close]; // WILL RELEASE THE NSWindow (AND ALL THE NSView'S INSIDE)
}

@end
