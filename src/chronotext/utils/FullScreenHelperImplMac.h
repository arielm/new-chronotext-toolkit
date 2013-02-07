#pragma once

#include "cinder/app/AppBasic.h"

class FullScreenHelperImplMac
{
    ci::app::AppBasic *app;
    
    bool fullScreen;
    ci::Vec2i windowPos;
    ci::Vec2i windowSize;
    
public:
    FullScreenHelperImplMac(ci::app::AppBasic *app) : app(app), fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
