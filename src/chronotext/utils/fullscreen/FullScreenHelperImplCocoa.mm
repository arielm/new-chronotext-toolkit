#include "FullScreenHelperImplCocoa.h"

#include "cinder/app/App.h"

#import <Cocoa/Cocoa.h>

using namespace ci;
using namespace ci::app;

bool FullScreenHelperImplCocoa::isFullScreen()
{
    return fullScreen;
}

void FullScreenHelperImplCocoa::toggleFullScreen()
{
    App *app = App::get();
    NSScreen *screen = app->getDisplay()->getNsScreen();
    NSView *view = (NSView*)app->getWindow()->getNative();
    
    if (fullScreen)
    {
        [view exitFullScreenModeWithOptions:nil];
    }
    else
    {
        NSApplicationPresentationOptions presentationOptions = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;
        
        NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithBool:NO], NSFullScreenModeAllScreens,
                                 [NSNumber numberWithUnsignedInteger:presentationOptions], NSFullScreenModeApplicationPresentationOptions,
                                 nil];
        
        [view enterFullScreenMode:screen withOptions:options];
    }
    
    fullScreen ^= true;
}
