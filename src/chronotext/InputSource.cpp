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
    InputSourceRef InputSource::getResource(const ci::fs::path &relativePath)
    {
        auto source = make_shared<InputSource>(TYPE_RESOURCE);
        source->relativePath = relativePath;
        source->filePathHint = relativePath.string();
        
#if defined(CINDER_COCOA)
        source->filePath = FileSystem::getResourcePath(relativePath);
#endif
        
        return source;
    }
    
    DataSourceRef InputSource::loadResource(const ci::fs::path &relativePath)
    {
        return InputSource::getResource(relativePath)->loadDataSource();
    }
    
    InputSourceRef InputSource::getResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        auto source = make_shared<InputSource>(TYPE_RESOURCE_MSW);
        source->mswID = mswID;
        source->mswType = mswType;
        source->filePathHint = resourceName;
        
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
    
    /*
     * TODO:
     * THERE IS PROBABLY A BETTER WAY TO HANDLE THE PARSING,
     * BUT Boost.Regex IS CURRENTLY NOT AN OPTION ON COCOA
     */
    InputSourceRef InputSource::get(const string &uri)
    {
        string scheme;
        string path;
        int mswID = -1;
        string mswType;
        
        auto found = uri.find("://");
        
        if (found != string::npos)
        {
            scheme = uri.substr(0, found);
            
            auto remainder1 = uri.substr(found + 3);
            auto found1 = remainder1.find("?id=");
            
            if (found1 != string::npos)
            {
                path = remainder1.substr(0, found1);
                
                auto remainder2 = remainder1.substr(found1 + 4);
                auto found2 = remainder2.find("&type=");
                
                if (found2 != string::npos)
                {
                    mswID = fromString<int>(remainder2.substr(0, found2));
                    mswType = remainder2.substr(found2 + 6);
                }
                else
                {
                    mswID = fromString<int>(remainder2);
                    mswType = "DATA";
                }
            }
            else
            {
                path = remainder1;
            }
        }
        
        if (!path.empty())
        {
            if (scheme == "res")
            {
                if ((mswID != -1) && !mswType.empty())
                {
                    return InputSource::getResource(path, mswID, mswType);
                }
                else
                {
                    return InputSource::getResource(path);
                }
            }
            else if (scheme == "file")
            {
                return InputSource::getFile(path);
            }
            else if (scheme == "assets")
            {
                return InputSource::getAsset(path);
            }
        }
        
        throw Exception("INVALID URI: " + uri);
    }
    
    DataSourceRef InputSource::load(const string &uri)
    {
        return InputSource::get(uri)->loadDataSource();
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
        return InputSource::getFile(filePath)->loadDataSource();
    }
    
    InputSourceRef InputSource::getFileInDocuments(const fs::path &relativePath)
    {
        auto source = InputSource::getFile(getDocumentsDirectory() / relativePath);
        source->relativePath = relativePath;
        
        return source;
    }
    
    DataSourceRef InputSource::loadFileInDocuments(const fs::path &relativePath)
    {
        return InputSource::getFileInDocuments(relativePath)->loadDataSource();
    }
    
    DataSourceRef InputSource::loadDataSource()
    {
        switch (type)
        {
            case TYPE_RESOURCE:
            {
#if defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
                AAsset* asset = AAssetManager_open(FileSystem::getAndroidAssetManager(), filePathHint.c_str(), AASSET_MODE_STREAMING);
                
                if (asset)
                {
                    AAsset_close(asset);
                    return DataSourceAsset::create(FileSystem::getAndroidAssetManager(), filePathHint);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
#elif defined(CINDER_ANDROID)
                try
                {
                    return app::loadResource(filePathHint); // TODO: TEST IF IT REALLY THROWS UPON ERROR
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
                }
#else
                if (fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                {
                    return DataSourcePath::create(filePath);
                }
                else
                {
                    throw Exception("RESOURCE NOT FOUND: " + relativePath.string());
                }
#endif
            }
                
            case TYPE_RESOURCE_MSW:
            {
                try
                {
                    return app::loadResource(filePathHint, mswID, mswType); // TODO: TEST IF IT REALLY THROWS UPON ERROR
                }
                catch (exception &e)
                {
                    throw Exception("RESOURCE NOT FOUND: " + filePathHint);
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
        switch (type)
        {
            case TYPE_FILE:
#if !defined(CINDER_ANDROID)
            case TYPE_ASSET:
#endif
#if defined(CINDER_COCOA)
            case TYPE_RESOURCE:
#endif
                return true;
        }
        
        return false;
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
         * COMPUTING THE VALUE ONLY ONCE ALLOWS FOR EFFICIENT USAGE IN std::map KEYS
         */
        if (uri.empty())
        {
            switch (type)
            {
                case TYPE_RESOURCE:
                    uri = "res://" + filePathHint;
                    break;
                    
                case TYPE_RESOURCE_MSW:
                    uri = "res://" + filePathHint + "?id=" + toString(mswID) + "&type=" + mswType;
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
