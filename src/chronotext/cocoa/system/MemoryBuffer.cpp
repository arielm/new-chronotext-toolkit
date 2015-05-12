/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cocoa/system/MemoryBuffer.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace memory
    {
        Buffer::~Buffer()
        {
            unlock();
        }
        
        bool Buffer::lock(InputSource::Ref inputSource)
        {
            unlock();
            
            if (inputSource->isFile())
            {
                locked = mappedFile.map(inputSource->getFilePath().c_str());
            }
            
            return locked;
        }
        
        void Buffer::unlock()
        {
            if (locked)
            {
                mappedFile.unmap();
                locked = false;
            }
        }
        
        const void* Buffer::data()
        {
            return mappedFile.data;
        }
        
        size_t Buffer::size()
        {
            return mappedFile.size;
        }
    }
}
