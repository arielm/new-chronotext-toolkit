/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA_TOUCH)

#include "chronotext/ios/system/DisplayHelper.h"

#elif defined(CINDER_ANDROID)

#include "chronotext/android/system/DisplayHelper.h"

#else

#include "chronotext/desktop/system/DisplayHelper.h"

#endif

namespace chr
{
    typedef display::Helper DisplayHelper;
}
