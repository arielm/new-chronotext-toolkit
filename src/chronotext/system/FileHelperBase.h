/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfo.h"

#include "cinder/Filesystem.h"

#include "boost/filesystem/fstream.hpp"

namespace chr
{
    namespace file
    {
        ci::fs::path getDefaultPath();
        
        class HelperBase
        {
        public:
            static int64_t getAvailableStorage(const ci::fs::path &folderPath = ci::fs::path());
            
            static ci::fs::path normalizePath(const ci::fs::path &absolutePath);
            static ci::fs::path relativizePath(const ci::fs::path &from, const ci::fs::path &to);
            static bool arePathsRelative(const ci::fs::path &ancestor, const ci::fs::path &heir);
            static ci::fs::path getFolderOrCreateIt(const ci::fs::path &folderPath);
        };
    }
}
