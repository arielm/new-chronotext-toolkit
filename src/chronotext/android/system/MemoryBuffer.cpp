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
        bool Buffer::lock(InputSource::Ref inputSource)
        {
            unlock();
            
            if (inputSource->isFile())
            {
                // TODO: HANDLE VIA posix/system/MappedFile
            }
            else
            {
                asset = AAssetManager_open(FileHelper::getAssetManager(), inputSource->getFilePath().c_str(), AASSET_MODE_BUFFER);
                
                if (asset)
                {
                    if (AAsset_isAllocated(asset)) // XXX: NEVER TRUE!
                    {
                        AAsset_close(asset);
                        asset = nullptr;
                        
                        locked = BufferBase::lock(inputSource);
                    }
                    else
                    {
                        locked = true;
                    }
                }
            }
            
            return locked;
        }
        
        void Buffer::unlock()
        {
            if (asset)
            {
                AAsset_close(asset);
                asset = nullptr;
                
                locked = false;
            }
            else
            {
                BufferBase::unlock();
            }
        }
        
        const void* Buffer::data()
        {
            return asset ? AAsset_getBuffer(asset) : BufferBase::data();
        }
        
        size_t Buffer::size()
        {
            return asset ? AAsset_getLength(asset) : BufferBase::size();
        }
    }
}
