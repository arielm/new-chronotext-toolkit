/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_ANDROID)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/system/FileHelperBase.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/InputSource.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace chr
{
    namespace file
    {
        class Helper : public HelperBase
        {
        public:
            static void setup(const system::InitInfo &initInfo);
            static void shutdown();

            static AAssetManager* getAssetManager();
            
            static ci::fs::path getInternalDataPath();
            static ci::fs::path getExternalDataPath();
            static ci::fs::path getApkPath();
            
            static AAsset* openAsset(const InputSource &inputSource, int mode = AASSET_MODE_UNKNOWN);
        };
    }
}
