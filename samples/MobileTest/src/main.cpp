#include "chronotext/cinder/main.h"

#pragma mark ----------------------------------------   IOS   ----------------------------------------

#if defined(CINDER_COCOA_TOUCH)

#include "AppDelegate.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#pragma mark ---------------------------------------- DESKTOP ----------------------------------------

#elif defined(CINDER_MAC) || defined(CINDER_MSW)

#include "Application.h"

CINDER_APP_NATIVE(Application, ci::app::RendererGl(ci::app::RendererGl::AA_NONE))

#endif

#pragma mark ---------------------------------------- SKETCH -----------------------------------------

#include "Sketch.h"

namespace chronotext
{
    CinderSketch* createSketch()
    {
        return new Sketch();
    }
    
    void destroySketch(CinderSketch *sketch)
    {
        delete sketch;
    }
}
