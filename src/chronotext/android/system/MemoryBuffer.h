/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 * 
 * 1) HANDLE REGULAR FILES (CURRENTLY: ONLY ANDROID ASSETS ARE SUPPORTED)
 *
 *
 * NOTES REGARDING ANDROID ASSETS:
 *
 * 1) THE BUFFER RETURNED BY data() WILL BE MEMORY-MAPPED ONLY IF:
 *    - THE CORRESPONDING ASSET IS UNCOMPRESSED AND "ALIGNED" INSIDE THE .APK
 *    - OTHERWISE: MEMORY-ALLOCATION WILL TAKE PLACE
 *
 * 2) PROPER ALIGNMENT IS AUTOMATICALLY HANDLED (CF zipalign INVOKED BY ant)
 *
 * 3) COMPRESSION IS AUTOMATICALLY SKIPPED FOR A PREDEFINED SET OF FILE-TYPES:
 *    - E.G. jpg, png, wav
 *
 * 4) THERE IS NO "TRIVIAL" WAY TO AVOID COMPRESSION FOR CUSTOM FILE-TYPES:
 *    - APART FROM ADDING A CUSTOM "nocompress" ENTRY IN sdk/tools/ant/build.xml
 *    - TODO: INVESTIGATE WITH GRADLE...
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_ANDROID)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/system/MemoryBufferBase.h"
#include "chronotext/system/FileHelper.h"

namespace chr
{
    namespace memory
    {
        class Buffer : public BufferBase
        {
        public:
            bool lock(const InputSource &inputSource) final;
            void unlock() final;
            
            const void* data() final;
            size_t size() final;
            
        protected:
            AAsset *asset = nullptr;
        };
    }
}
