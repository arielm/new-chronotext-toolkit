#include "chronotext/InputSource.h"

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;
using namespace app;

InputSourceRef InputSource::getResource(const std::string &resourceName)
{
    InputSource *source = new InputSource(TYPE_RESOURCE);
    source->resourceName = resourceName;

#if defined(CINDER_MSW)
    source->filePath = fs::path("resources") / resourceName;
#elif defined(CINDER_COCOA)
    source->filePath = App::getResourcePath(resourceName);
#endif
    
#if defined(CINDER_MSW)
    source->filePathHint = source->filePath.string();
#else
    source->filePathHint = resourceName;
#endif
    
    return InputSourceRef(source);
}

DataSourceRef InputSource::loadResource(const std::string &resourceName)
{
    return InputSource::getResource(resourceName)->loadDataSource();
}

InputSourceRef InputSource::getResource(const string &resourceName, int mswID, const std::string &mswType)
{
#if defined(CINDER_MSW)
    InputSource *source = new InputSource(TYPE_RESOURCE_MSW);
#else
    InputSource *source = new InputSource(TYPE_RESOURCE);
#endif

    source->resourceName = source->filePathHint = resourceName;
    
#if defined(CINDER_MSW)
    source->mswID = mswID;
    source->mswType = mswType;
#elif defined(CINDER_COCOA)
    source->filePath = App::getResourcePath(resourceName);
#endif
    
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

InputSourceRef InputSource::getFile(const fs::path &filePath)
{
    InputSource *source = new InputSource(TYPE_FILE);
    source->filePath = filePath;
    source->filePathHint = filePath.string();
    
    return InputSourceRef(source);
}

DataSourceRef InputSource::loadDataSource()
{
    switch (type)
    {
        case TYPE_RESOURCE:
#if defined(CINDER_MSW)
            return DataSourcePath::create(filePath);
#elif defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
            CI_LOGI("LOADING ASSET: %s", resourceName.c_str());
            return DataSourceAsset::create(gAssetManager, resourceName);
#else
            return app::loadResource(resourceName);
#endif

        case TYPE_RESOURCE_MSW:
            return app::loadResource(resourceName, mswID, mswType);
            
        case TYPE_FILE:
            return DataSourcePath::create(filePath);
    }
    
    return DataSourceRef();
}

bool InputSource::isFile()
{
    return !filePath.empty();
}

fs::path InputSource::getFilePath()
{
    return filePath;
}

string InputSource::getFilePathHint()
{
    return filePathHint;
}

string InputSource::getUniqueName()
{
    switch (type)
    {
        case TYPE_RESOURCE:
            return "RESOURCE:" + resourceName;

        case TYPE_RESOURCE_MSW:
            return "RESOURCE_MSW:" + boost::lexical_cast<string>(mswID) + ":" + mswType;
            
        case TYPE_FILE:
            return "FILE:" + filePath.string();
    }

    return "";
}

#if defined(CINDER_ANDROID)
void InputSource::setAndroidAssetManager(AAssetManager *assetManager)
{
    gAssetManager = assetManager;
}

AAssetManager* InputSource::getAndroidAssetManager()
{
    return gAssetManager;
}
#endif
