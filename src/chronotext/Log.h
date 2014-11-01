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
 *    - TAGS:
 *      - LOG << TAG(foo) << "baz";
 *    - TIME-STAMPS:
 *      - LOG << TIMESTAMP << "baz";
 *    - THREAD-ID
 *      - LOG << THREAD_ID << "baz";
 *    - FOR ANDROID: THE IMPLEMENTATION SHOULD BE ADAPTED TO logcat
 *
 * 2) ADDITIONAL "LOG TARGETS":
 *    - UNIX TERMINALS
 *    - FILES
 */

#pragma once

#include "cinder/Cinder.h"

#include <iostream>

/*
 * FROM: https://github.com/SuperV1234/SSVUtils/blob/master/include/SSVUtils/Core/Log/Log.hpp
 */
using CoutType = decltype(std::cout);
using StdEndLine = CoutType&(CoutType&);

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
            std::lock_guard<std::mutex> lg(mtx);
            
            log::cout() << value;
            return *this;
        }
            
        Log& operator<<(Tag tag);
        Log& operator<<(StdEndLine manip);
        
    protected:
        std::mutex mtx;
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
