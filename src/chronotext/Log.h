/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

namespace chronotext
{
#if defined(CINDER_MSW)
    static ci::msw::dostream *CHROUT;
#elif defined(CINDER_ANDROID)
    static ci::android::dostream *CHROUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
    static mac::dostream *CHROUT;
#endif
    
    static inline std::ostream& chrout()
    {
#if defined(CINDER_MSW)
        if (!CHROUT)
        {
            CHROUT = new ci::msw::dostream;
        }
        return *CHROUT;
#elif defined(CINDER_ANDROID)
        if (!CHROUT)
        {
            CHROUT = new ci::android::dostream;
        }
        return *CHROUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
        if (!CHROUT)
        {
            CHROUT = new mac::dostream;
        }
        return *CHROUT;
#else
        return std::cout;
#endif
    }
}

namespace chr = chronotext;

#define LOGI chr::chrout()
#define LOGI_IF(COND) (COND) && LOGI

#if defined(DEBUG) || defined(FORCE_LOG)
#define LOGD LOGI
#define LOGD_IF(COND) LOGI_IF(COND)
#else
#define LOGD false && chr::chrout()
#define LOGD_IF(COND) false && chr::chrout()
#endif

// TODO: ADD LOGX AND LOGX_IF FOR "DATA REPORTING"
