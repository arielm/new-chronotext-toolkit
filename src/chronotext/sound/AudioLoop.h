#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA)
    #include "chronotext/sound/AudioLoopImplCocoa.h"
    typedef AudioLoopImplCocoa AudioLoop;
#elif defined(CINDER_MSW)
    #include "chronotext/sound/AudioLoopImplMsw.h"
    typedef AudioLoopImplMsw AudioLoop;
#endif
