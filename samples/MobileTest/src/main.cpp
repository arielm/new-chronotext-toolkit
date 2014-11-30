#include "cinder/Cinder.h"

#if defined(CINDER_COCOA_TOUCH)

#pragma mark ----------------------------------------   IOS   ----------------------------------------

#include "AppDelegate.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#elif defined(CINDER_ANDROID)

#pragma mark ---------------------------------------- ANDROID ----------------------------------------

#include <jni.h>

extern "C"
{
	void android_main(struct android_app *state)
    {}
}

#elif defined(CINDER_MAC) || defined(CINDER_MSW)

#pragma mark ---------------------------------------- DESKTOP ----------------------------------------

#include "Application.h"

CINDER_APP_NATIVE(Application, ci::app::RendererGl(ci::app::RendererGl::AA_NONE))

#else

#error UNSUPPORTED PLATFORM

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
