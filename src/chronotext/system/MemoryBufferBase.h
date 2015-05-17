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
 * 1) THERE SHOULD BE A UNIQUE SET OF BufferBases PER InputSource
 *    - NECESSARY ON OSX, iOS AND ANDROID
 *    - EVERYTHING COULD BE HANDLED VIA MemoryManager
 *
 * 2) THERE COULD BE SOME "TYPE" INFO, E.G.
 *    - ANDROID_ASSET_MEMORY_MAPPED
 *    - ANDROID_ASSET_COMPRESSED
 *    - MSW_RESOURCE
 *    - POSIX_FILE_MEMORY_MAPPED
 *
 * 3) THERE COULD BE SOME "ATTRIBUTES" INFO, EG.
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
            
            virtual bool lock(const InputSource &inputSource);
            virtual void unlock();
            
            virtual const void* data();
            virtual size_t size();
            
        protected:
            bool locked = false;
            ci::Buffer buffer;
        };
    }
}
