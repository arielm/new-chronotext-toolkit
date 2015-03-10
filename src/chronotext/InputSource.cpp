/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/InputSource.h"
#include "chronotext/system/FileHelper.h"

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace inputsource
    {
        const string EMPTY_STRING = "";
        const fs::path EMPTY_PATH = "";
    }
    
    InputSource::InputSource(Type type)
    :
    type(type)
    {}
    
    InputSource::Ref InputSource::getResource(const fs::path &relativePath)
    {
        auto source = Ref(new InputSource(TYPE_RESOURCE));
        source->relativePath = relativePath;
        source->filePathHint = relativePath.string();
        
#if defined(CINDER_COCOA)
        source->filePath = FileHelper::getResourcePath(relativePath);
#endif
        
        return source;
    }
    
    DataSourceRef InputSource::loadResource(const fs::path &relativePath)
    {
        return InputSource::getResource(relativePath)->loadDataSource();
    }
    
    InputSource::Ref InputSource::getResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        auto source = Ref(new InputSource(TYPE_RESOURCE_MSW));
        source->mswID = mswID;
        source->mswType = mswType;
        source->relativePath = resourceName;
        source->filePathHint = resourceName;
        
        return source;
    }
    
    DataSourceRef InputSource::loadResource(const string &resourceName, int mswID, const std::string &mswType)
    {
        return InputSource::getResource(resourceName, mswID, mswType)->loadDataSource();
    }
    
    InputSource::Ref InputSource::getAsset(const fs::path &relativePath)
    {
        auto source = Ref(new InputSource(TYPE_ASSET));
        source->relativePath = relativePath;
        source->filePathHint = relativePath.string();

#if defined(CINDER_MAC) || defined(CINDER_MSW)
        source->filePath = app::getAssetPath(relativePath);
#elif defined(CINDER_COCOA_TOUCH)
        source->filePath = FileHelper::getResourcePath("assets" / relativePath);
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
    InputSource::Ref InputSource::get(const string &uri)
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
        
        throw EXCEPTION(InputSource, "INVALID URI: " + uri);
    }
    
    DataSourceRef InputSource::load(const string &uri)
    {
        return InputSource::get(uri)->loadDataSource();
    }
    
    InputSource::Ref InputSource::getFile(const fs::path &filePath)
    {
        auto source = Ref(new InputSource(TYPE_FILE));
        source->filePath = filePath;
        source->filePathHint = filePath.string();
        
        return source;
    }
    
    DataSourceRef InputSource::loadFile(const fs::path &filePath)
    {
        return InputSource::getFile(filePath)->loadDataSource();
    }
    
    InputSource::Ref InputSource::getFileInDocuments(const fs::path &relativePath)
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
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            switch (type)
            {
                case TYPE_RESOURCE:
                {
#if defined(CINDER_ANDROID)
                    auto asset = AAssetManager_open(FileHelper::getAssetManager(), relativePath.c_str(), AASSET_MODE_STREAMING);
                    
                    if (asset)
                    {
                        AAsset_close(asset);
                        return DataSourceAsset::create(FileHelper::getAssetManager(), relativePath.c_str());
                    }
                    else
                    {
                        throw EXCEPTION(InputSource, "RESOURCE NOT FOUND: " + filePathHint);
                    }
#else
                    if (fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                    {
                        return DataSourcePath::create(filePath);
                    }
                    else
                    {
                        throw EXCEPTION(InputSource, "RESOURCE NOT FOUND: " + filePathHint);
                    }
#endif
                }
                    
                case TYPE_RESOURCE_MSW:
                {
                    try
                    {
                        return app::loadResource(relativePath.string(), mswID, mswType); // TODO: VERIFY IF IT THROWS UPON ERROR
                    }
                    catch (exception &e)
                    {
                        throw EXCEPTION(InputSource, "RESOURCE NOT FOUND: " + filePathHint);
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
                        throw EXCEPTION(InputSource, "FILE NOT FOUND: " + filePathHint);
                    }
                }
                    
                case TYPE_ASSET:
                {
#if defined(CINDER_ANDROID)
                    auto resourcePath = "assets" / relativePath;
                    auto asset = AAssetManager_open(FileHelper::getAssetManager(), resourcePath.c_str(), AASSET_MODE_STREAMING);
                    
                    if (asset)
                    {
                        AAsset_close(asset);
                        return DataSourceAsset::create(FileHelper::getAssetManager(), resourcePath.c_str());
                    }
                    else
                    {
                        throw EXCEPTION(InputSource, "ASSET NOT FOUND: " + filePathHint);
                    }
#else
                    if (fs::exists(filePath)) // NECESSARY, BECAUSE THE FOLLOWING WON'T THROW IF FILE DOESN'T EXIST
                    {
                        return DataSourcePath::create(filePath);
                    }
                    else
                    {
                        throw EXCEPTION(InputSource, "ASSET NOT FOUND: " + filePathHint);
                    }
#endif
                }
            }
        }
        
        throw EXCEPTION(InputSource, "INVALID INPUT-SOURCE");
    }
    
    InputSource::Ref InputSource::getSubSource(const fs::path &subPath)
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            switch (type)
            {
                case TYPE_FILE:
                {
                    return getFile(filePath.parent_path() / subPath);
                }
                    
                case TYPE_ASSET:
                {
                    return getAsset(relativePath.parent_path() / subPath);
                }
                    
                case TYPE_RESOURCE:
                {
                    return getResource(relativePath.parent_path() / subPath);
                }
                    
                case TYPE_RESOURCE_MSW:
                {
                    /*
                     * TODO: PARSE "resourceName", "mswID" AND "mswType"
                     */
                    break;
                }
            }
        }
        
        throw EXCEPTION(InputSource, "INVALID INPUT-SOURCE");
    }
    
    bool InputSource::isFile() const
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
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
        }
        
        return false;
    }
    
    fs::path InputSource::getFilePath() const
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            return filePath;
        }
        
        return inputsource::EMPTY_PATH;
    }
    
    string InputSource::getFilePathHint() const
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            return filePathHint;
        }
        
        return inputsource::EMPTY_STRING;
    }
    
    void InputSource::setFilePathHint(const string &hint)
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            filePathHint = hint;
        }
    }
    
    InputSource::Type InputSource::getType() const
    {
        return type;
    }
    
    const string& InputSource::getURI()
    {
        if (this) // EXTRA-PROTECTION AGAINST NON-INITIALIZED InputSource::Refs
        {
            /*
             * COMPUTING THE VALUE ONLY ONCE ALLOWS FOR EFFICIENT USAGE IN std::map KEYS
             */
            
            if (uri.empty())
            {
                switch (type)
                {
                    case TYPE_RESOURCE:
                        uri = "res://" + relativePath.string();
                        break;
                        
                    case TYPE_RESOURCE_MSW:
                        uri = "res://" + relativePath.string() + "?id=" + toString(mswID) + "&type=" + mswType;
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
        
        return inputsource::EMPTY_STRING;
    }
}
