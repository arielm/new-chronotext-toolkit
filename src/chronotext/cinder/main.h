#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_ANDROID)

#include "chronotext/android/cinder/JNI.h"

extern "C"
{
	void android_main(struct android_app *state)
    {}
}

#elif !defined(CINDER_COCOA_TOUCH) && !defined(CINDER_MAC) && !defined(CINDER_MSW)

#error UNSUPPORTED PLATFORM

#endif

namespace chronotext
{
    class CinderSketch;
    
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
}
