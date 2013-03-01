#pragma once

#include "cinder/Cinder.h"

#if defined(CHR_COMPLEX) && defined(CINDER_COCOA_TOUCH)
    #include "chronotext/ios/cinder/CinderSketchComplex.h"
    typedef CinderSketchComplex CinderSketch;
#elif defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
    #include "chronotext/android/cinder/CinderSketchComplex.h"
    typedef CinderSketchComplex CinderSketch;
#else
    #include "chronotext/cinder/CinderSketchSimple.h"
    typedef CinderSketchSimple CinderSketch;
#endif
