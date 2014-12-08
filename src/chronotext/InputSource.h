/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Exception.h"

#include "cinder/DataSource.h"

namespace chr
{
    class InputSource
    {
    public:
        typedef std::shared_ptr<InputSource> Ref;

        enum Type
        {
            TYPE_RESOURCE,
            TYPE_RESOURCE_MSW,
            TYPE_FILE,
            TYPE_ASSET
        };

        /*
         * TO USE WITH THE "CINDER_RESOURCE" MACRO OR WITH A RESOURCE-NAME,
         * FOR FILES LOCATED IN THE BUNDLE ON OSX OR IOS,
         * OR DATA EMBEDDED INSIDE THE APK ON ANDROID
         *
         * OSX, IOS: isFile() WILL RETURN true
         * ANDROID: isFile() WILL RETURN false
         */
        static InputSource::Ref getResource(const ci::fs::path &relativePath);
        static ci::DataSourceRef loadResource(const ci::fs::path &relativePath);
        
        /*
         * TO USE WITH THE "CINDER_RESOURCE" MACRO,
         * FOR DATA EMBEDDED INSIDE THE EXECUTABLE ON MSW
         *
         * isFile() WILL RETURN false
         */
        static InputSource::Ref getResource(const std::string &resourceName, int mswID, const std::string &mswType);
        static ci::DataSourceRef loadResource(const std::string &resourceName, int mswID, const std::string &mswType);
        
        /*
         * MSW AND OSX:
         * - TO USE WITH REGULAR "CINDER ASSETS"
         * - isFile() WILL RETURN true
         *
         * IOS:
         * - TO USE WITH CINDER'S "IOS ASSET FOLDER"
         * - isFile() WILL RETURN true
         *
         * ANDROID:
         * - CREATE A SHORTCUT TO THE "assets" FOLDER IN "resources"
         * - THE ASSETS WILL BE PACKAGED INSIDE THE APK
         * - isFile() WILL RETURN false
         */
        static InputSource::Ref getAsset(const ci::fs::path &relativePath);
        static ci::DataSourceRef loadAsset(const ci::fs::path &relativePath);

        static InputSource::Ref get(const std::string &uri);
        static ci::DataSourceRef load(const std::string &uri);

        static InputSource::Ref getFile(const ci::fs::path &filePath);
        static ci::DataSourceRef loadFile(const ci::fs::path &filePath);

        static InputSource::Ref getFileInDocuments(const ci::fs::path &relativePath);
        static ci::DataSourceRef loadFileInDocuments(const ci::fs::path &relativePath);

        InputSource(Type type)
        :
        type(type)
        {}

        ci::DataSourceRef loadDataSource();
        InputSource::Ref getSubSource(const ci::fs::path &subPath);
        
        bool isFile() const;
        ci::fs::path getFilePath() const;
        
        std::string getFilePathHint() const;
        void setFilePathHint(const std::string &hint);

        std::string getURI();

    protected:
        int type;
        int mswID;
        std::string mswType;
        ci::fs::path filePath;
        ci::fs::path relativePath;
        std::string filePathHint;
        std::string uri;
    };
}
