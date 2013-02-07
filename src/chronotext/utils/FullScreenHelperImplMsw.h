#pragma once

#include "cinder/app/AppBasic.h"

class FullScreenHelperImplMsw
{
    ci::app::AppBasic *app;
    
    bool fullScreen;
    ci::Vec2i windowPos;
    ci::Vec2i windowSize;
    
public:
    FullScreenHelperImplMsw(ci::app::AppBasic *app) : app(app), fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
