/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
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

namespace chronotext
{
    namespace log
    {
        std::ostream& cout()
        {
#if defined(CINDER_MSW)
            static msw::dostream COUT;
            return COUT;
#elif defined(CINDER_ANDROID)
            static android::dostream COUT;
            return COUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
            static mac::dostream COUT;
            return COUT;
#else
            return std::cout;
#endif
        }
    }
    
    Log& Log::instance()
    {
        static Log instance;
        return instance;
    }
    
    Log& Log::operator<<(Tag tag)
    {
        lock_guard<mutex> lg(mtx);
        
        log::cout() << "[" << tag.value << "] ";
        return *this;
    }
    
    Log& Log::operator<<(StdEndLine manip)
    {
        lock_guard<mutex> lg(mtx);

        log::cout() << endl;
        return *this;
    }
}
