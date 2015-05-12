/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

namespace chr
{
    namespace memory
    {
        class BufferBase
        {
        public:
            virtual ~BufferBase() {};
            
            virtual bool lock(InputSource::Ref inputSource) = 0;
            virtual void unlock() = 0;
            
            virtual const void* data() = 0;
            virtual size_t size() = 0;
        };
    }
}
