#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

#import "cinder/app/AppImplCocoaRendererGl.h"

/*
 * THIS CATEGORY IS FOR RESIZING OUR OverlayWindow BEFORE THE "RESIZE SIGNAL" IS SENT
 * IT WOULD NOT NOT BE POSSIBLE TO ACHIEVE, SAY, USING NSWindowDidResizeNotification
 *
 * REFERENCES:
 * https://github.com/cinder/Cinder/blob/c894b2a81eb4d859070b177f989f60b470e92b8c/src/cinder/app/CinderView.mm#L197
 * http://stackoverflow.com/a/8971179/50335
 */

@interface AppImplCocoaRendererGl (ResizeHack)

- (void) overridenDefaultResize;

@end

@implementation AppImplCocoaRendererGl (ResizeHack)

- (void) overridenDefaultResize
{
    [self overridenDefaultResize]; // NO, THIS WON'T LEAD TO A STACK-OVERFLOW (THANKS TO THE "METHOD SWIZZLING")
    
    NSRect frame = [cinderView.window convertRectToScreen:cinderView.bounds];
    NSWindow *overlayWindow = [[cinderView.window childWindows] objectAtIndex:0];
    [overlayWindow setFrame:frame display:YES];
}

+ (void) load
{
    method_exchangeImplementations(class_getInstanceMethod(self, @selector(defaultResize)), class_getInstanceMethod(self, @selector(overridenDefaultResize)));
}

@end

@interface OverlayWindow : NSWindow

- (id) initWithParentWindow:(NSWindow*)window;
- (BOOL) canBecomeKeyWindow;

@end
