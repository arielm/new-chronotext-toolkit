/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/Log.h"

#if defined(CINDER_MSW)

#include "cinder/msw/OutputDebugStringStream.h"

#elif defined(CINDER_ANDROID)

#include "cinder/android/LogStream.h"

#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)

#include "chronotext/utils/SyslogStringStream.h"

#endif

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
        static ci::mac::dostream COUT;
        return COUT;
#else
        return std::cout;
#endif
    }
    
    Log& Log::instance()
    {
        static Log instance;
        return instance;
    }
    
    Log& Log::operator<<(ostream_manipulator manip)
    {
        boost::mutex::scoped_lock lock(mtx);

        Log::cout() << manip;
        return *this;
    }
}
