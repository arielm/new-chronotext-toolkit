/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/system/MemoryBuffer.h"
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
            
            if (!inputSource->isFile())
            {
                asset = AAssetManager_open(FileHelper::getAssetManager(), inputSource->getFilePath().c_str(), AASSET_MODE_STREAMING);
                locked = asset && !AAsset_isAllocated(asset);
            }
            else
            {
                // TODO: HANDLE VIA "REGULAR" MEMORY-MAPPING (CF cocoa/system/MemoryBuffer)
            }
            
            return locked;
        }
        
        void Buffer::unlock()
        {
            if (locked)
            {
                AAsset_close(asset);
                locked = false;
            }
        }
        
        const void* Buffer::data()
        {
            return locked ? AAsset_getBuffer(asset) : nullptr;
        }
        
        size_t Buffer::size()
        {
            return locked ? AAsset_getLength(asset) : 0;
        }
    }
}
