/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/FileSystem.h"

#include "cinder/app/App.h"

#if defined(CINDER_ANDROID)
#include <sys/statfs.h>
#elif defined(CINDER_COCOA)
#include <sys/mount.h>
#endif

using namespace std;
using namespace ci;
using namespace app;

namespace chronotext
{
    /*
     * FIXME: USE boost::filesystem::space_info() INSTEAD
     */
    int64_t FileSystem::getAvailableStorage(const fs::path &folderPath)
    {
#if defined(CINDER_MSW)
        return 0; // TODO
#else
        struct statfs stat;
        statfs(folderPath.string().c_str(), &stat);
        
        return (int64_t)stat.f_bavail * (int64_t)stat.f_bsize;
#endif
    }

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
    
    /*
     * PATH RELATIVIZATION WITHOUT THE NEED FOR FILES TO EXIST
     * REFERENCE: http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path
     */
    fs::path FileSystem::relativizePath(const fs::path &from, const fs::path &to)
    {
        fs::path a_From = fs::absolute(from);
        fs::path a_To = fs::absolute(to);
        
        fs::path ret;
        fs::path::const_iterator itrFrom(a_From.begin()), itrTo(a_To.begin());
        
        // Find common base
        for (fs::path::const_iterator toEnd(a_To.end()), fromEnd(a_From.end()); itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo);
        
        // Navigate backwards in directory to reach previously found base
        for (fs::path::const_iterator fromEnd(a_From.end()); itrFrom != fromEnd; ++itrFrom)
        {
            if ((*itrFrom) != ".")
            {
                ret /= "..";
            }
        }
        
        // Now navigate down the directory branch
        for (; itrTo != a_To.end() ; ++itrTo)
        {
            ret /= *itrTo;
        }
        
        return ret;
    }
    
    bool FileSystem::arePathsRelative(const fs::path &ancestor, const fs::path &heir)
    {
        if (ancestor.empty())
        {
            return true;
        }
        else
        {
            fs::path parent = heir;
            
            while (!parent.empty())
            {
                if (parent == ancestor)
                {
                    return true;
                }
                
                parent = parent.parent_path();
            }
            
            return false;
        }
    }
    
    fs::path FileSystem::getFolderOrCreateIt(const fs::path &folderPath)
    {
        if (!fs::exists(folderPath))
        {
            fs::create_directories(folderPath);
        }
        
        return folderPath;
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
    fs::path FileSystem::getResourcePath(const fs::path &relativePath)
    {
#if defined(CHR_COMPLEX)
        return FileSystem::getResourcePath() / relativePath;
#else
        return App::getResourcePath(relativePath);
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
    
    void FileSystem::setAndroidInternalDataPath(const string &dataPath)
    {
        gInternalDataPath = fs::path(dataPath);
    }
    
    fs::path FileSystem::getAndroidInternalDataPath()
    {
        return gInternalDataPath;
    }
    
    void FileSystem::setAndroidExternalDataPath(const string &dataPath)
    {
        gExternalDataPath = fs::path(dataPath);
    }
    
    fs::path FileSystem::getAndroidExternalDataPath()
    {
        return gExternalDataPath;
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
