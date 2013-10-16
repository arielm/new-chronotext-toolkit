/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Filesystem.h"

#include <fstream>
#include <memory>

#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
#include <android/asset_manager.h>
#endif

namespace chronotext
{
    class FileSystem
    {
    public:
        static int64_t getAvailableStorage(const ci::fs::path &folderPath);
        
        static ci::fs::path normalizePath(const ci::fs::path &absolutePath);
        static ci::fs::path getFolderOrCreateIt(const ci::fs::path &folderPath);
        
        static std::shared_ptr<std::ofstream> getOFStream(const ci::fs::path &filePath);
        static std::shared_ptr<std::ifstream> getIFStream(const ci::fs::path &filePath);
        
#if defined(CINDER_COCOA)
        static ci::fs::path getResourcePath();
        static ci::fs::path getResourcePath(const std::string &resourceName);
#endif
        
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
        static void setAndroidAssetManager(AAssetManager *assetManager);
        static AAssetManager* getAndroidAssetManager();
        
        static void setAndroidInternalDataPath(const std::string &internalDataPath);
        static ci::fs::path getAndroidInternalDataPath();
        
        static void setAndroidApkPath(const std::string &apkPath);
        static ci::fs::path getAndroidApkPath();
#endif
    };
    
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
    static AAssetManager *gAssetManager;
    static ci::fs::path gInternalDataPath;
    static ci::fs::path gApkPath;
#endif
}

namespace chr = chronotext;
