/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_MSW)
#include "cinder/msw/OutputDebugStringStream.h"
#elif defined(CINDER_ANDROID)
#include "cinder/android/LogStream.h"
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
#include "chronotext/utils/SyslogStringStream.h"
#endif

namespace chronotext
{
    static inline std::ostream& cout()
    {
#if defined(CINDER_MSW)
        static ci::msw::dostream COUT;
        return COUT;
#elif defined(CINDER_ANDROID)
        static ci::android::dostream COUT;
        return COUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
        static mac::dostream COUT;
        return COUT;
#else
        return std::cout;
#endif
    }
}

namespace chr = chronotext;

#if defined(DISCARD_LOGI)
#define LOGI false && chr::cout()
#define LOGI_IF(COND) false && chr::cout()
#else
#define LOGI chr::cout()
#define LOGI_IF(COND) (COND) && chr::cout()
#endif

#if defined(DEBUG) || defined(FORCE_LOGD)
#define LOGD chr::cout()
#define LOGD_IF(COND) (COND) && chr::cout()
#else
#define LOGD false && chr::cout()
#define LOGD_IF(COND) false && chr::cout()
#endif
