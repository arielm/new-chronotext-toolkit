/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <cstddef>

namespace chr
{
    namespace memory
    {
        struct MappedFile
        {
            size_t size = 0;
            void *data = nullptr;

            MappedFile() = default;
            MappedFile(const MappedFile &other) = delete;
            void operator=(const MappedFile &other) = delete;
            
            ~MappedFile();
            
            bool map(const char *filePath);
            void unmap();
        };
    }
}
