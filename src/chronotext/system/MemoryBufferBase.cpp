/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/MemoryBufferBase.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace memory
    {
        BufferBase::~BufferBase()
        {
            unlock();
        }
        
        bool BufferBase::lock(const InputSource &inputSource)
        {
            unlock();
            
            buffer = inputSource.loadDataSource()->getBuffer();
            locked = (buffer.getDataSize() > 0);
            
            return locked;
        }
        
        void BufferBase::unlock()
        {
            if (locked)
            {
                buffer.reset();
                locked = false;
            }
        }
        
        const void* BufferBase::data()
        {
            return locked ? buffer.getData() : nullptr;
        }
        
        size_t BufferBase::size()
        {
            return locked ? buffer.getDataSize() : 0;
        }
    }
}
