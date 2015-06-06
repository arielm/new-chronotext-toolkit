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
 * 1) THERE COULD BE SOME "TYPE" INFO, E.G.
 *    - ANDROID_ASSET_MEMORY_MAPPED
 *    - ANDROID_ASSET_ALLOCATED
 *    - MSW_RESOURCE_MEMORY_MAPPED
 *    - POSIX_FILE_MEMORY_MAPPED
 *
 * 2) THERE COULD BE SOME "ATTRIBUTES" INFO, EG.
 *    - READ_ONLY
 *    - SHARED
 *    - PRIVATE (?)
 *    - ALLOCATED (?)
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/Buffer.h"

namespace chr
{
    namespace memory
    {
        class BufferBase
        {
        public:
            virtual ~BufferBase();
            
            virtual bool lock(InputSource &inputSource);
            virtual void unlock();
            
            virtual const void* data();
            virtual size_t size();
            
        protected:
            bool locked = false;
            ci::Buffer buffer;
        };
    }
}
