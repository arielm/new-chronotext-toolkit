#pragma once

#include "cinder/Vector.h"

class FullScreenHelperImplMac
{
    bool fullScreen;
    ci::Vec2i windowPos;
    ci::Vec2i windowSize;
    
public:
    FullScreenHelperImplMac() : fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
