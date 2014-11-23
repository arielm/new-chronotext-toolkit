/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) ON ALL THE PLATFORMS:
 *    - SYNTAX (NON-FINAL)::
 *      - TAGS:
 *        - LOG << TAG("foo") << "baz";
 *      - TIME-STAMPS:
 *        - LOG << TIMESTAMP << "baz";
 *      - THREAD-ID
 *        - LOG << THREAD_ID << "baz";
 *    - FOR ANDROID: THE IMPLEMENTATION SHOULD BE ADAPTED TO logcat
 *    - NESTED INDENTATION SYSTEM
 *
 * 2) ADDITIONAL "LOG TARGETS":
 *    - UNIX TERMINALS
 *    - FILES
 *
 * 3) BEFORE PROGRESSING FURTHER:
 *    - CHECK boost::iostreams CAN BE USED
 */

#pragma once

#include "cinder/Cinder.h"

#include <iostream>

#include <boost/thread/mutex.hpp>

/*
 * FROM: http://stackoverflow.com/a/1136617/50335
 */
typedef std::ostream& (*ostream_manipulator)(std::ostream&);

namespace chronotext
{
    namespace log
    {
       std::ostream& cout();
    }

    struct Tag
    {
        std::string value;
    };
    
    class Log
    {
    public:
        static Log& instance();
        
        template <class T>
        Log& operator<<(const T &value)
        {
            boost::mutex::scoped_lock lock(mtx);
            
            log::cout() << value;
            return *this;
        }
        
        Log& operator<<(Tag tag);
        Log& operator<<(ostream_manipulator manip);
        
    protected:
        boost::mutex mtx;
    };
}

namespace chr = chronotext;

#if defined(DISCARD_LOGI)
#define LOGI false && chr::log::cout()
#define LOGI_IF(COND) false && chr::log::cout()
#else
#define LOGI chr::log::cout()
#define LOGI_IF(COND) (COND) && chr::log::cout()
#endif

#if defined(DEBUG) || defined(FORCE_LOGD)
#define LOGD chr::log::cout()
#define LOGD_IF(COND) (COND) && chr::log::cout()
#else
#define LOGD false && chr::log::cout()
#define LOGD_IF(COND) false && chr::log::cout()
#endif

#define LOG chr::Log::instance()
#define LOG_IF (COND) (COND) && chr::Log::instance()
            
#define TAG(VALUE) chr::Tag{VALUE}
