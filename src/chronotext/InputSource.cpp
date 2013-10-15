/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/InputSource.h"

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;
using namespace app;

namespace chronotext
{
    /*
     * PATH NORMALIZATION WITHOUT THE NEED FOR FILES TO EXIST
     * REFERENCE: http://stackoverflow.com/a/12797413/50335
     */
    fs::path InputSource::normalizePath(const fs::path &absolutePath)
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
    
    InputSourceRef InputSource::getResource(const std::string &resourceName)
    {
        InputSource *source = new InputSource(TYPE_RESOURCE);
        source->resourceName = source->filePathHint = resourceName;
        
#if defined(CINDER_COCOA)
        source->filePath = getResourcePath(resourceName);
#endif
        
        return InputSourceRef(source);
    }
    
    DataSourceRef InputSource::loadResource(const std::string &resourceName)
    {
        return InputSource::getResource(resourceName)->loadDataSource();
    }
    
    InputSourceRef InputSource::getResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        InputSource *source = new InputSource(TYPE_RESOURCE_MSW);
        source->mswID = mswID;
        source->mswType = mswType;
        source->resourceName = source->filePathHint = resourceName;
        
        return InputSourceRef(source);
    }
    
    DataSourceRef InputSource::loadResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        return InputSource::getResource(resourceName, mswID, mswType)->loadDataSource();
    }
    
    InputSourceRef InputSource::getFileInDocuments(const std::string &relativePath)
    {
        return InputSource::getFile(getDocumentsDirectory() / relativePath);
    }
    
    DataSourceRef InputSource::loadFileInDocuments(const std::string &relativePath)
    {
        return DataSourcePath::create(getDocumentsDirectory() / relativePath);
    }
    
    InputSourceRef InputSource::getFile(const fs::path &filePath)
    {
        InputSource *source = new InputSource(TYPE_FILE);
        source->filePath = filePath;
        source->filePathHint = filePath.string();
        
        return InputSourceRef(source);
    }
    
    DataSourceRef InputSource::loadFile(const fs::path &filePath)
    {
        return DataSourcePath::create(filePath);
    }
    
    DataSourceRef InputSource::loadDataSource()
    {
        switch (type)
        {
            case TYPE_RESOURCE:
            {
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
                AAsset* asset = AAssetManager_open(gAssetManager, resourceName.c_str(), AASSET_MODE_STREAMING);
                
                if (asset)
                {
                    AAsset_close(asset);
                    return DataSourceAsset::create(gAssetManager, resourceName);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
#elif defined(CINDER_ANDROID)
                try
                {
                    return app::loadResource(resourceName);
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
#else
                if (fs::exists(filePath))
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
#endif
            }
                
            case TYPE_RESOURCE_MSW:
            {
                try
                {
                    return app::loadResource(resourceName, mswID, mswType);
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
            }
                
            case TYPE_FILE:
            {
                if (fs::exists(filePath))
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("FILE NOT FOUND: " + filePathHint);
                }
            }
        }
        
        return DataSourceRef();
    }
    
    bool InputSource::isFile() const
    {
        return !filePath.empty();
    }
    
    fs::path InputSource::getFilePath() const
    {
        return filePath;
    }
    
    string InputSource::getFilePathHint() const
    {
        return filePathHint;
    }
    
    string InputSource::getUniqueName() const
    {
        switch (type)
        {
            case TYPE_RESOURCE:
                return "res://" + resourceName;
                
            case TYPE_RESOURCE_MSW:
                return "res_msw://" + boost::lexical_cast<string>(mswID) + "/" + mswType;
                
            case TYPE_FILE:
                if (fs::exists(filePath))
                {
                    return "file://" + fs::canonical(filePath).string();
                }
                else
                {
                    return "undefined://" + filePathHint;
                }
        }
        
        return "";
    }
    
    void InputSource::setFilePathHint(const string &hint)
    {
        filePathHint = hint;
    }
    
#if defined(CINDER_COCOA)
    fs::path InputSource::getResourcePath()
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
    fs::path InputSource::getResourcePath(const string &resourceName)
    {
#if defined(CHR_COMPLEX)
        return getResourcePath() / resourceName;
#else
        return App::getResourcePath(resourceName);
#endif
    }
#endif
    
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
    
    void InputSource::setAndroidAssetManager(AAssetManager *assetManager)
    {
        gAssetManager = assetManager;
    }
    
    AAssetManager* InputSource::getAndroidAssetManager()
    {
        return gAssetManager;
    }
    
    void InputSource::setAndroidInternalDataPath(const string &internalDataPath)
    {
        gInternalDataPath = fs::path(internalDataPath);
    }
    
    fs::path InputSource::getAndroidInternalDataPath()
    {
        return gInternalDataPath;
    }
    
    void InputSource::setAndroidApkPath(const string &apkPath)
    {
        gApkPath = fs::path(apkPath);
    }
    
    fs::path InputSource::getAndroidApkPath()
    {
        return gApkPath;
    }
    
#endif
}
