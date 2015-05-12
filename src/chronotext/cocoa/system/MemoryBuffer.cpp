/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cocoa/system/MemoryBuffer.h"
#include "chronotext/Context.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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
                auto fd = open(inputSource->getFilePath().c_str(), O_RDONLY);
                
                if (fd > -1)
                {
                    struct stat buf;
                    fstat(fd, &buf);
                    
                    if (buf.st_size > 0)
                    {
                        fileSize = buf.st_size;
                        fileMap = mmap(0, fileSize, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
                        locked = (fileMap != MAP_FAILED);
                    }
                    
                    close(fd);
                }
            }
            
            return locked;
        }
        
        void Buffer::unlock()
        {
            if (locked)
            {
                munmap(fileMap, fileSize);
                locked = false;
            }
        }
        
        const void* Buffer::data()
        {
            return locked ? fileMap : nullptr;
        }
        
        size_t Buffer::size()
        {
            return locked ? fileSize : 0;
        }
    }
}
