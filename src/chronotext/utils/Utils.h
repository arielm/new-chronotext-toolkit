#pragma once

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#if defined(CINDER_MSW)
#include "cinder/msw/OutputDebugStringStream.h"
#elif defined(CINDER_ANDROID)
#include "cinder/android/LogStream.h"
#endif

#if defined(CINDER_MSW)
static cinder::msw::dostream *CHROUT;
#elif defined(CINDER_ANDROID)
static cinder::android::dostream *CHROUT;
#endif

static inline std::ostream& chrout()
{
#if defined(CINDER_MSW)
    if (!CHROUT)
    {
        CHROUT = new cinder::msw::dostream;
    }
    return *CHROUT;
#elif defined(CINDER_ANDROID)
    if (!CHROUT)
    {
        CHROUT = new cinder::android::dostream;
    }
    return *CHROUT;
#else
    return std::cout;
#endif
}

#define LOGI chrout()

#if defined(DEBUG) || defined(FORCE_LOG)
#define LOGD chrout()
#else
#define LOGD false && chrout()
#endif

// ---

/*
 * cout REDIRECTION (FOR COCOA ONLY)
 * AS DESCRIBED IN http://www.cplusplus.com/reference/iostream/ios/rdbuf/
 */

static std::streambuf *gCoutBackup = NULL;
static std::ofstream gCoutFilestr;

static void logToFile(const ci::fs::path &filePath)
{
    if (!gCoutBackup)
    {
        gCoutFilestr.open(filePath.c_str());
        gCoutBackup = std::cout.rdbuf();
        
        std::streambuf *psbuf = gCoutFilestr.rdbuf();
        std::cout.rdbuf(psbuf);
    }
}

static void logToConsole()
{
    if (gCoutBackup)
    {
        std::cout.rdbuf(gCoutBackup);
        gCoutFilestr.close();
        gCoutBackup = NULL;
    }
}

// ---

static int search(float *array, float value, int min, int max)
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

static inline int search(const std::vector<float> &array, float value, int min, int max)
{
    return search((float*)array.data(), value, min, max);
}

std::string wstringToUtf8(const std::wstring &s);
std::wstring utf8ToWstring(const std::string &s);
