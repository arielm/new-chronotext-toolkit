/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cocoa/system/FileHelper.h"

#include "cinder/app/App.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    namespace file
    {
        /*
         * TODO:
         *
         * CONSIDER SWITCHING TO SOME (SIMPLIER) OBJECTIVE-C-BASED SOLUTION (E.G. NSBundle::pathForResource)
         *
         * PREVIOUSLY: THE GOAL WAS TO AVOID FORCING USERS TO DECLARE (THE UNIQUE) FileHelper.cpp AS OBJECTIVE-C++ ON COCOA
         * NOW: THE IMPLEMENTATION IS PLATFORM-SPECIFIC (I.E. FileHelper.mm IS AUTOMATICALLY DECLARED AS OBJECTIVE-C++)
         */
        
        fs::path Helper::getResourcePath()
        {
#if defined(CINDER_COCOA_TOUCH)
            CFBundleRef bundle = CFBundleGetMainBundle();
            CFURLRef url = CFBundleCopyBundleURL(bundle);
            CFStringRef tmp = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
            
            CFIndex length = CFStringGetLength(tmp);
            CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
            char *buffer = (char*)malloc(maxSize);
            CFStringGetCString(tmp, buffer, maxSize, kCFStringEncodingUTF8);
            
            fs::path path(buffer);
            
            CFRelease(url);
            CFRelease(tmp);
            free(buffer);
            
            return path;
#else
            return App::getResourcePath();
#endif
        }
        
        fs::path Helper::getResourcePath(const fs::path &relativePath)
        {
#if defined(CINDER_COCOA_TOUCH)
            return Helper::getResourcePath() / relativePath;
#else
            return App::getResourcePath(relativePath);
#endif
        }
    }
}
