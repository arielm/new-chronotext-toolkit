/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Log.h"

#include "cinder/Cinder.h"

#if defined(CINDER_MSW)

#include "cinder/msw/OutputDebugStringStream.h"

#elif defined(CINDER_ANDROID)

#include "cinder/android/LogStream.h"

#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)

#include "chronotext/utils/SyslogStringStream.h"

#endif

#include <mutex>

using namespace std;
using namespace ci;

namespace chr
{
    ostream& Log::cout()
    {
#if defined(CINDER_MSW)
        static ci::msw::dostream COUT;
        return COUT;
#elif defined(CINDER_ANDROID)
        static ci::android::dostream COUT;
        return COUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
        static chr::mac::dostream COUT;
        return COUT;
#else
        return std::cout;
#endif
    }
    
    Log::~Log()
    {
        static mutex _mutex;
        lock_guard<mutex> lock(_mutex);
        
        Log::cout() << ss.rdbuf() << std::flush;
    }
    
    Log& Log::operator<<(ostream_manipulator manip)
    {
        ss << manip;
        return *this;
    }
}
