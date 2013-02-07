#pragma once

#include "cinder/Vector.h"

class FullScreenHelperImplMsw
{
    bool fullScreen;
    ci::Vec2i windowPos;
    ci::Vec2i windowSize;
    
public:
    FullScreenHelperImplMsw() : fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
