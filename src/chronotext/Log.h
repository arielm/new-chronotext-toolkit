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
 *    - CHECK IF Google Logging Library (glog) CAN BE USED:
 *      https://google-glog.googlecode.com/svn/trunk/doc/glog.html
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

#include <sstream>
#include <iostream>

namespace chr
{
    class Log
    {
    public:
        typedef std::ostream& (*ostream_manipulator)(std::ostream&); // FROM: http://stackoverflow.com/a/1136617/50335

        static std::ostream& cout();
        
        ~Log();
        
        template <typename T>
        Log& operator<<(const T &value)
        {
            ss << value;
            return *this;
        }
        
        Log& operator<<(ostream_manipulator manip);
        
        explicit operator bool() const { return true; }
        
    protected:
        std::stringstream ss;
    };
}

#if defined(DISCARD_LOGI)
#   define LOGI false && chr::Log()
#   define LOGI_IF(COND) false && chr::Log()
#   define LOGI_FN(FN, ...) {}
#else
#   define LOGI chr::Log()
#   define LOGI_IF(COND) (COND) && chr::Log()
#   define LOGI_FN(FN, ...) { chr::Log __LOG__; FN(__LOG__, ##__VA_ARGS__); }
#endif

#if defined(DEBUG) || defined(FORCE_LOGD)
#   define LOGD chr::Log()
#   define LOGD_IF(COND) (COND) && chr::Log()
#   define LOGD_FN(FN, ...) {}
#else
#   define LOGD false && chr::Log()
#   define LOGD_IF(COND) false && chr::Log()
#   define LOGD_FN(FN, ...) { chr::Log __LOG__; FN(__LOG__, ##__VA_ARGS__); }
#endif
