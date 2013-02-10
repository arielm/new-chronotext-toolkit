#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_MAC)

#include "FullScreenHelperImplCocoa.h"
typedef FullScreenHelperImplCocoa FullScreenHelper;

#else

#include "cinder/app/App.h"

class FullScreenHelper
{
public:
    bool isFullScreen()
    {
        return ci::app::App::get()->isFullScreen();
    }
    
    void toggleFullScreen()
    {
        ci::app::App *app = ci::app::App::get();
        app->setFullScreen(!app->isFullScreen());
    }
};

#endif
