/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA)
    #include "chronotext/incubator/sound/AudioLoopImplCocoa.h"
    typedef AudioLoopImplCocoa AudioLoop;
#elif defined(CINDER_MSW)
    #include "chronotext/incubator/sound/AudioLoopImplMsw.h"
    typedef AudioLoopImplMsw AudioLoop;
#endif
