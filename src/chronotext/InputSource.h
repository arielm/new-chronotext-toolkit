#pragma once

#include "cinder/DataSource.h"

#if defined(CINDER_ANDROID)
#include <android/asset_manager.h>
static AAssetManager *gAssetManager;
#endif

typedef std::shared_ptr<class InputSource> InputSourceRef;

class InputSource
{
    int type;
    
    std::string resourceName;
    int mswID;
    std::string mswType;
    ci::fs::path filePath;
    std::string filePathHint;
    
    InputSource(int type) : type(type) {}

public:
    enum
    {
        TYPE_RESOURCE,
        TYPE_RESOURCE_MSW,
        TYPE_FILE
    };
    
    /*
     * FOR "DYNAMIC" RESOURCES
     *
     * OSX, IOS AND MSW: isFile() WILL RETURN true
     * ANDROID: isFile() WILL RETURN false
     *
     * MSW:
     * - UPON DEPLOYMENT: THE FILE IS SUPPOSED TO BE PLACED INSIDE A resources FOLDER ALONGSIDE THE EXECUTABLE
     * - DURING DEVELOPMENT: YOU WILL HAVE TO SET "Configuration Properties / Debugging / Working Directory" TO "$(ProjectDir).."
     *   FOR BOTH "DEBUG" AND "RELEASE" MODES
     *   NOTE THAT THIS IS PERSISTED IN YOUR PROJECT'S .vcxproj.user FILE (HENCE GITHUB MUST BE TOLD NOT TO IGNORE SUCH FILES)
     */
    static InputSourceRef getResource(const std::string &resourceName);
    static ci::DataSourceRef loadResource(const std::string &resourceName);
    
    /*
     * FOR "STATIC" RESOURCES (I.E. EMBEDDED INSIDE THE EXECUTABLE ON MSW)
     *
     * OSX AND IOS: isFile() WILL RETURN true
     * ANDROID AND MSW: isFile() WILL RETURN false
     *
     * THE "CHR_RESOURCE" MACRO IS INTENDED TO BE USED HERE (SEE ChronotextResource.h):
     * - WE CAN'T USE THE "CINDER_RESOURCE" MACRO, OTHERWISE getFilePathHint() WOULD NOT WORK
     * - getFilePathHint() IS NECESSARY, E.G. IN ORDER TO BE ABLE TO DEFINE A FILE'S EXTENSION
     */
    static InputSourceRef getResource(const std::string &resourceName, int mswID, const std::string &mswType);
    static ci::DataSourceRef loadResource(const std::string &resourceName, int mswID, const std::string &mswType);
    
    static InputSourceRef getFileInDocuments(const std::string &relativePath);
    static InputSourceRef getFile(const ci::fs::path &filePath);
    
    ci::DataSourceRef loadDataSource();
    
    bool isFile();
    ci::fs::path getFilePath();
    
    std::string getFilePathHint();
    std::string getUniqueName();
    
#if defined(CINDER_ANDROID)
    static void setAndroidAssetManager(AAssetManager *assetManager);
    static AAssetManager* getAndroidAssetManager();
#endif
};
