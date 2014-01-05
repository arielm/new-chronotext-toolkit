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

namespace chronotext
{
    InputSourceRef InputSource::getResource(const std::string &resourceName)
    {
        auto source = make_shared<InputSource>(TYPE_RESOURCE);
        source->resourceName = source->filePathHint = resourceName;
        
#if defined(CINDER_COCOA)
        source->filePath = FileSystem::getResourcePath(resourceName);
#endif
        
        return source;
    }
    
    DataSourceRef InputSource::loadResource(const std::string &resourceName)
    {
        return InputSource::getResource(resourceName)->loadDataSource();
    }
    
    InputSourceRef InputSource::getResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        auto source = make_shared<InputSource>(TYPE_RESOURCE_MSW);
        source->mswID = mswID;
        source->mswType = mswType;
        source->resourceName = source->filePathHint = resourceName;
        
        return source;
    }
    
    DataSourceRef InputSource::loadResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        return InputSource::getResource(resourceName, mswID, mswType)->loadDataSource();
    }
    
    InputSourceRef InputSource::getAsset(const fs::path &relativePath)
    {
        auto source = make_shared<InputSource>(TYPE_ASSET);
        source->relativePath = relativePath;
        source->filePathHint = relativePath.string();

#if defined(CINDER_MAC) || defined(CINDER_MSW)
        source->filePath = app::getAssetPath(relativePath);
#elif defined(CINDER_COCOA_TOUCH)
        source->filePath = FileSystem::getResourcePath("assets" / relativePath);
#endif
        
        return source;
    }
    
    DataSourceRef InputSource::loadAsset(const fs::path &relativePath)
    {
        return InputSource::getAsset(relativePath)->loadDataSource();
    }
    
    InputSourceRef InputSource::getFileInDocuments(const fs::path &relativePath)
    {
        return InputSource::getFile(getDocumentsDirectory() / relativePath);
    }
    
    DataSourceRef InputSource::loadFileInDocuments(const fs::path &relativePath)
    {
        return DataSourcePath::create(getDocumentsDirectory() / relativePath);
    }
    
    InputSourceRef InputSource::getFile(const fs::path &filePath)
    {
        auto source = make_shared<InputSource>(TYPE_FILE);
        source->filePath = filePath;
        source->filePathHint = filePath.string();
        
        return source;
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
                AAsset* asset = AAssetManager_open(FileSystem::getAndroidAssetManager(), resourceName.c_str(), AASSET_MODE_STREAMING);
                
                if (asset)
                {
                    AAsset_close(asset);
                    return DataSourceAsset::create(FileSystem::getAndroidAssetManager(), resourceName);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + resourceName);
                }
#elif defined(CINDER_ANDROID)
                try
                {
                    return app::loadResource(resourceName); // TODO: TEST IF IT REALLY THROWS UPON ERROR
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + resourceName);
                }
#else
                if (fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + resourceName);
                }
#endif
            }
                
            case TYPE_RESOURCE_MSW:
            {
                try
                {
                    return app::loadResource(resourceName, mswID, mswType); // TODO: TEST IF IT REALLY THROWS UPON ERROR
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + resourceName);
                }
            }
                
            case TYPE_FILE:
            {
                if (fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("FILE NOT FOUND: " + filePath.string());
                }
            }
                
            case TYPE_ASSET:
            {
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
                string resourcePath = ("assets" / relativePath).string();
                AAsset* asset = AAssetManager_open(FileSystem::getAndroidAssetManager(), resourcePath.c_str(), AASSET_MODE_STREAMING);
                
                if (asset)
                {
                    AAsset_close(asset);
                    return DataSourceAsset::create(FileSystem::getAndroidAssetManager(), resourcePath);
                }
                else
                {
                    throw Exception("ASSET NOT FOUND: " + relativePath.string());
                }
#elif defined(CINDER_ANDROID)
                try
                {
                    return app::loadResource(("assets" / relativePath).string()); // TODO: TEST IF IT REALLY THROWS UPON ERROR
                }
                catch (exception &e)
                {
                    throw Exception("ASSET NOT FOUND: " + relativePath.string());
                }
#else
                if (!filePath.empty() && fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("ASSET NOT FOUND: " + relativePath.string());
                }
#endif
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
    
    void InputSource::setFilePathHint(const string &hint)
    {
        filePathHint = hint;
    }
    
    string InputSource::getURI()
    {
        /*
         * ADVANTAGES OF COMPUTING THE VALUE ONLY ONCE:
         * - ALLOWS EFFICIENT USAGE IN std::map KEYS
         * - WILL MAKE SENSE WHEN WE START CREATING InputSource INSTANCES BASED ON URIS
         */
        if (uri.empty())
        {
            switch (type)
            {
                case TYPE_RESOURCE:
                    uri = "res://" + resourceName;
                    break;
                    
                case TYPE_RESOURCE_MSW:
                    uri = "res://" + resourceName + "?id=" + toString(mswID) + "&type=" + mswType;
                    break;
                    
                case TYPE_FILE:
                    uri = "file://" + filePath.string();
                    break;
                    
                case TYPE_ASSET:
                    uri = "assets://" + relativePath.string();
                    break;
            }
        }
        
        return uri;
    }
}
