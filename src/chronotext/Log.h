/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) BEFORE PROGRESSING FURTHER:
 *    - CHECK IF boost::iostreams CAN BE USED
 *    - STUDY THE "VARIADIC LOG" EXAMPLE FROM "New Tools for Class and Library Authors":
 *      https://github.com/boostcon/cppnow_presentations_2012/raw/master/wed/schurr_cpp11_tools_for_class_authors.pdf
 *
 * 2) FEATURES:
 *    - POSSIBILITY TO DEFINE "TAGS" AND "ATTRIBUTES":
 *      - E.G. (NON-FINAL SYNTAX):
 *        LOG << TAG("some app") << ATTRIBUTES(TIMESTAMP, THREAD_ID) << "some message" << endl;
 *      - ON ANDROID: ADAPT TO logcat
 *    - NESTED INDENTATION
 *
 * 3) ADDITIONAL "LOG TARGETS":
 *    - UNIX TERMINALS
 *    - FILES
 */

#pragma once

#include "cinder/Cinder.h"

#include <iostream>

#include <boost/thread/mutex.hpp>

namespace chr
{
    class Log
    {
    public:
        typedef std::ostream& (*ostream_manipulator)(std::ostream&); // FROM: http://stackoverflow.com/a/1136617/50335

        static std::ostream& cout();
        static Log& instance();
        
        template <typename T>
        Log& operator<<(const T &value)
        {
            boost::mutex::scoped_lock lock(mtx);
            
            Log::cout() << value;
            return *this;
        }
        
        Log& operator<<(ostream_manipulator manip);
        
    protected:
        boost::mutex mtx;
    };
}

#if defined(DISCARD_LOGI)
#define LOGI false && chr::Log::cout()
#define LOGI_IF(COND) false && chr::Log::cout()
#else
#define LOGI chr::Log::cout()
#define LOGI_IF(COND) (COND) && chr::Log::cout()
#endif

#if defined(DEBUG) || defined(FORCE_LOGD)
#define LOGD chr::Log::cout()
#define LOGD_IF(COND) (COND) && chr::Log::cout()
#else
#define LOGD false && chr::Log::cout()
#define LOGD_IF(COND) false && chr::Log::cout()
#endif

#define LOG chr::Log::instance()
#define LOG_IF (COND) (COND) && chr::Log::instance()
