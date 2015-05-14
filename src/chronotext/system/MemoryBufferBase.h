/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
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
