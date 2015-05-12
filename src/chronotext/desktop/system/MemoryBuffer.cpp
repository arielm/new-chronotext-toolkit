/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/desktop/system/MemoryBuffer.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace memory
    {
        bool Buffer::lock(InputSource::Ref inputSource)
        {
            unlock();
            
            buffer = inputSource->loadDataSource()->getBuffer();
            locked = buffer.getDataSize() > 0;
            
            return locked;
        }
        
        void Buffer::unlock()
        {
            if (locked)
            {
                buffer.reset();
                locked = false;
            }
        }
        
        const void* Buffer::data()
        {
            return locked ? buffer.getData() : nullptr;
        }
        
        size_t Buffer::size()
        {
            return locked ? buffer.getDataSize() : 0;
        }
    }
}
