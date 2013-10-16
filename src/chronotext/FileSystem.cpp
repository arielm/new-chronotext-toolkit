/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/FileSystem.h"
#include "chronotext/utils/Utils.h"

#include "cinder/app/App.h"

#if defined(CINDER_ANDROID)
#include <sys/statfs.h>
#else
#include <sys/mount.h>
#endif

using namespace std;
using namespace ci;
using namespace app;

namespace chronotext
{
    /*
     * PATH NORMALIZATION WITHOUT THE NEED FOR FILES TO EXIST
     * REFERENCE: http://stackoverflow.com/a/12797413/50335
     */
    fs::path FileSystem::normalizePath(const fs::path &absolutePath)
    {
        fs::path::iterator it = absolutePath.begin();
        fs::path result = *it++;
        
        for (; it != absolutePath.end(); ++it)
        {
            if (*it == "..")
            {
                result = result.parent_path();
            }
            else if (*it != ".")
            {
                result /= *it;
            }
        }
        
        return result;
    }
    
    int64_t FileSystem::getAvailableStorage(const fs::path &folderPath)
    {
        struct statfs stat;
        statfs(folderPath.string().c_str(), &stat);
        
        return (int64_t)stat.f_bavail * (int64_t)stat.f_bsize;
    }
    
#if defined(CINDER_COCOA)
    fs::path FileSystem::getResourcePath()
    {
#if defined(CHR_COMPLEX)
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
#endif
    
#if defined(CINDER_COCOA)
    fs::path FileSystem::getResourcePath(const string &resourceName)
    {
#if defined(CHR_COMPLEX)
        return getResourcePath() / resourceName;
#else
        return App::getResourcePath(resourceName);
#endif
    }
#endif
    
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
    
    void FileSystem::setAndroidAssetManager(AAssetManager *assetManager)
    {
        gAssetManager = assetManager;
    }
    
    AAssetManager* FileSystem::getAndroidAssetManager()
    {
        return gAssetManager;
    }
    
    void FileSystem::setAndroidInternalDataPath(const string &internalDataPath)
    {
        gInternalDataPath = fs::path(internalDataPath);
    }
    
    fs::path FileSystem::getAndroidInternalDataPath()
    {
        return gInternalDataPath;
    }
    
    void FileSystem::setAndroidApkPath(const string &apkPath)
    {
        gApkPath = fs::path(apkPath);
    }
    
    fs::path FileSystem::getAndroidApkPath()
    {
        return gApkPath;
    }
    
#endif
}
