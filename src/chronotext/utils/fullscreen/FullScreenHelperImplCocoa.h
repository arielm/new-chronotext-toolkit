#pragma once

#include "cinder/Vector.h"

class FullScreenHelperImplCocoa
{
    bool fullScreen;
    ci::Vec2i windowPos;
    ci::Vec2i windowSize;
    
public:
    FullScreenHelperImplCocoa() : fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
