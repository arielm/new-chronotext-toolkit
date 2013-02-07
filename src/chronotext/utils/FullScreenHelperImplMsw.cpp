#include "FullScreenHelperImplMsw.h"

#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;

bool FullScreenHelperImplMsw::isFullScreen()
{
    return fullScreen;
}

void FullScreenHelperImplMsw::toggleFullScreen()
{
    AppBasic *app = AppBasic::get();
    
	if (fullScreen)
    {
        setFullScreen(false);
        
        app->setWindowSize(windowSize.x, windowSize.y);
        app->setWindowPos(windowPos);
    }
    else
    {
        windowSize = app->getWindowSize();
        windowPos = app->getWindowPos();
        
        setFullScreen(true);
    }
    
    fullScreen ^= true;
}
