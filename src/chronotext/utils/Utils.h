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
    static ci::msw::dostream *COUT;
#elif defined(CINDER_ANDROID)
    static ci::android::dostream *COUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
    static mac::dostream *COUT;
#endif
    
    static inline std::ostream& cout()
    {
#if defined(CINDER_MSW)
        if (!COUT)
        {
            COUT = new ci::msw::dostream;
        }
        return *COUT;
#elif defined(CINDER_ANDROID)
        if (!COUT)
        {
            COUT = new ci::android::dostream;
        }
        return *COUT;
#elif defined(CINDER_MAC) && defined(FORCE_SYSLOG)
        if (!COUT)
        {
            COUT = new mac::dostream;
        }
        return *COUT;
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
            cout().rdbuf(coutBackup);
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
        coutBackup = cout().rdbuf();
        
        std::streambuf *psbuf = coutFileStream.rdbuf();
        cout().rdbuf(psbuf);
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
    
    std::string loadUtf8(ci::DataSourceRef source);
    std::wstring loadWstring(ci::DataSourceRef source);
    
    // ---
    
    std::string hexDump(const char *data, int size);
    std::string prettyBytes(uint64_t numBytes, int precision = 2);
    std::string percent(float ratio, int precision = 1);
}

namespace chr = chronotext;

#define LOGI chr::cout()

#if defined(DEBUG) || defined(FORCE_LOG)
#define LOGD chr::cout()
#else
#define LOGD false && chr::cout()
#endif
