/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/system/MemoryBufferBase.h"

namespace chr
{
    namespace memory
    {
        class Buffer : public BufferBase
        {
        public:
            ~Buffer() final;
            
            bool lock(InputSource::Ref inputSource) final;
            void unlock() final;
            
            const void* data() final;
            size_t size() final;
            
        protected:
            bool locked = false;
            size_t fileSize;
            void *fileMap;
        };
    }
}
