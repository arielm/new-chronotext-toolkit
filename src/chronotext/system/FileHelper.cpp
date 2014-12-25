/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FileHelper.h"

#include "cinder/app/App.h"

using namespace std;
using namespace ci;
using namespace app;

namespace chr
{
    /*
     * TODO: TEST ON WINDOWS
     */
    int64_t FileHelper::getAvailableStorage(const fs::path &folderPath)
    {
        fs::path target;
        
        if (folderPath.empty())
        {
#if defined(CINDER_ANDROID)
            target = getAndroidInternalDataPath();
#else
            target = fs::current_path();
#endif
        }
        
        if (fs::exists(target))
        {
            return fs::space(target).available;
        }
        else
        {
            return 0;
        }
    }

    /*
     * PATH NORMALIZATION WITHOUT THE NEED FOR FILES TO EXIST
     * REFERENCE: http://stackoverflow.com/a/12797413/50335
     */
    fs::path FileHelper::normalizePath(const fs::path &absolutePath)
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
    fs::path FileHelper::relativizePath(const fs::path &from, const fs::path &to)
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
    
    bool FileHelper::arePathsRelative(const fs::path &ancestor, const fs::path &heir)
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
    
    fs::path FileHelper::getFolderOrCreateIt(const fs::path &folderPath)
    {
        if (!fs::exists(folderPath))
        {
            fs::create_directories(folderPath);
        }
        
        return folderPath;
    }
    
#if defined(CINDER_COCOA)
    fs::path FileHelper::getResourcePath()
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
#endif
    
#if defined(CINDER_COCOA)
    fs::path FileHelper::getResourcePath(const fs::path &relativePath)
    {
#if defined(CINDER_COCOA_TOUCH)
        return FileHelper::getResourcePath() / relativePath;
#else
        return App::getResourcePath(relativePath);
#endif
    }
#endif
    
#if defined(CINDER_ANDROID)
    
    void FileHelper::setAndroidAssetManager(AAssetManager *assetManager)
    {
        gAssetManager = assetManager;
    }
    
    AAssetManager* FileHelper::getAndroidAssetManager()
    {
        return gAssetManager;
    }
    
    void FileHelper::setAndroidInternalDataPath(const string &dataPath)
    {
        gInternalDataPath = fs::path(dataPath);
    }
    
    fs::path FileHelper::getAndroidInternalDataPath()
    {
        return gInternalDataPath;
    }
    
    void FileHelper::setAndroidExternalDataPath(const string &dataPath)
    {
        gExternalDataPath = fs::path(dataPath);
    }
    
    fs::path FileHelper::getAndroidExternalDataPath()
    {
        return gExternalDataPath;
    }
    
    void FileHelper::setAndroidApkPath(const string &apkPath)
    {
        gApkPath = fs::path(apkPath);
    }
    
    fs::path FileHelper::getAndroidApkPath()
    {
        return gApkPath;
    }
    
#endif
}
