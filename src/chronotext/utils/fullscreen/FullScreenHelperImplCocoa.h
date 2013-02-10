#pragma once

class FullScreenHelperImplCocoa
{
    bool fullScreen;
    
public:
    FullScreenHelperImplCocoa() : fullScreen(false) {}
    
    bool isFullScreen();
    void toggleFullScreen();
};
