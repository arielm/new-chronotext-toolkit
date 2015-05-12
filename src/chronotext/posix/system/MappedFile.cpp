/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/posix/system/MappedFile.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

namespace chr
{
    namespace memory
    {
        MappedFile::~MappedFile()
        {
            unmap();
        }
        
        bool MappedFile::map(const char *filePath)
        {
            unmap();
            
            auto fd = open(filePath, O_RDONLY);
            
            if (fd != -1)
            {
                struct stat stats;
                
                if ((fstat(fd, &stats) != -1) && (stats.st_size > 0))
                {
                    size = stats.st_size;
                    data = mmap(nullptr, size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
                    
                    close(fd);
                    
                    if (data != MAP_FAILED)
                    {
                        return true;
                    }
                    
                    size = 0;
                    data = nullptr;
                }
            }
            
            return false;
        }
        
        void MappedFile::unmap()
        {
            if (data && size)
            {
                munmap(data, size);
                
                size = 0;
                data = nullptr;
            }
        }
    }
}
