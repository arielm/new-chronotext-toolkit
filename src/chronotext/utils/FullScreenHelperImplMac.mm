#include "FullScreenHelperImplMac.h"

#include "cinder/app/AppBasic.h"

#import <Cocoa/Cocoa.h>

using namespace ci;
using namespace ci::app;

bool FullScreenHelperImplMac::isFullScreen()
{
    return fullScreen;
}

void FullScreenHelperImplMac::toggleFullScreen()
{
    AppBasic *app = AppBasic::get();
    
    if (fullScreen)
    {
        app->setWindowSize(windowSize.x, windowSize.y);
        app->setWindowPos(windowPos);
        
        app->setBorderless(false);
        
        NSApplicationPresentationOptions options = NSApplicationPresentationDefault;
        [NSApp setPresentationOptions:options];
    }
    else
    {
        windowSize = app->getWindowSize();
        windowPos = app->getWindowPos();
        
        app->setBorderless(true);
        
        Area screenArea = app->getDisplay().getArea();
        app->setWindowSize(screenArea.getWidth(), screenArea.getHeight());
        app->setWindowPos(0, 0);
        
        NSApplicationPresentationOptions options = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;
        [NSApp setPresentationOptions:options];
    }
    
    fullScreen ^= true;
}
