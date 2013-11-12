/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

#if defined(CINDER_MSW)
#include "cinder/msw/OutputDebugStringStream.h"
#elif defined(CINDER_ANDROID)
#include "cinder/android/LogStream.h"
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
#include "chronotext/utils/SyslogStringStream.h"
#endif

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
    
    // ---
    
    /*
     * cout REDIRECTION, AS DESCRIBED IN http://www.cplusplus.com/reference/iostream/ios/rdbuf/
     */
    
    static std::streambuf *coutBackup = NULL;
    static std::ofstream coutFileStream;
    
    static void endCoutRedirection()
    {
        if (coutBackup)
        {
            chrout().rdbuf(coutBackup);
            coutFileStream.close();
            coutBackup = NULL;
        }
    }
    
    static void redirectCoutToFile(const ci::fs::path &filePath)
    {
        if (coutBackup)
        {
            endCoutRedirection();
        }
        
        coutFileStream.open(filePath.string().c_str());
        coutBackup = chrout().rdbuf();
        
        std::streambuf *psbuf = coutFileStream.rdbuf();
        chrout().rdbuf(psbuf);
    }
    
    // ---
    
    template<typename T>
    static int search(T *array, T value, int min, int max)
    {
        int mid = (min + max) >> 1;
        
        while (min < mid)
        {
            if (array[mid - 1] < value)
            {
                min = mid;
            }
            else if (array[mid - 1] > value)
            {
                max = mid;
            }
            else
            {
                min = max = mid;
            }
            mid = (min + max) >> 1;
        }
        
        return mid - 1;
    }
    
    template<typename T>
    static inline int search(const std::vector<T> &array, float value, int min, int max)
    {
        return search((T*)array.data(), value, min, max);
    }
    
    // ---
    
    std::string wstringToUtf8(const std::wstring &s);
    std::wstring utf8ToWstring(const std::string &s);

    template<typename T> T loadString(ci::DataSourceRef source);
    template<typename T> std::vector<T> readLines(ci::DataSourceRef source);
    
    std::vector<std::string> readInstructions(chr::InputSourceRef source);
    
    // ---
    
    std::string hexDump(const char *data, int size);
    std::string prettyBytes(uint64_t numBytes, int precision = 2);
    std::string percent(float ratio, int precision = 1);
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
