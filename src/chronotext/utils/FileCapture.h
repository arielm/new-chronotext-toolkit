/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCE: http://stackoverflow.com/a/956269/50335
 *
 * SHOULD WORK AS-IS ON POSIX PLATFORMS (FULLY TESTED ON OSX)
 *
 * TODO:
 *
 * 1) TEST FURTHER ON iOS AND ANDROID
 *
 * 2) ENHANCE FLUSHING:
 *    - IT SHOULD NOT BE SIZE-BOUND
 *    - IT SHOULD BE POSSIBLE TO "FLUSH AS MEMORY-BUFFER"
 *
 * 3) ADAPT TO NON-UNIX PLATFORMS (I.E. WINDOWS)
 */

#pragma once

#include "chronotext/Exception.h"

#include <unistd.h>
#include <memory>

namespace chronotext
{
    class FileCapture
    {
    public:
        constexpr static size_t DEFAULT_BUFFER_SIZE = 256;
        
        FileCapture(FILE *target = 0)
        :
        initialized(false),
        target(target)
        {
            int target_fd = fileno(target);
            
            if (target_fd >= 0)
            {
                if (pipe(pipe_fd) >= 0)
                {
                    backup_fd = dup(target_fd); // CREATING TARGET DUPLICATE
                    dup2(pipe_fd[1], target_fd); // REPLACING TARGET
                    
                    initialized = true;
                }
            }
            
            if (!initialized)
            {
                throw EXCEPTION(FileCapture, "INITIALIZATION FAILED");
            }
        }
        
        ~FileCapture()
        {
            if (initialized)
            {
                fflush(target);
                
                close(pipe_fd[1]);
                dup2(backup_fd, target_fd); // RESTORING TARGET
                close(backup_fd); // RELEASING TARGET DUPLICATE
            }
        }
        
        std::string flushAsString(bool trimTrailing = false, size_t bufferSize = DEFAULT_BUFFER_SIZE) const
        {
            if (initialized)
            {
                auto buffer = std::unique_ptr<char[]>(new char[bufferSize]()); // ZERO-FILLED
                
                fflush(target);
                read(pipe_fd[0], buffer.get(), bufferSize);
                
                // ---
                
                size_t length = strlen(buffer.get());
                
                if (trimTrailing && (length < bufferSize))
                {
                    while ((length > 0) && (buffer[length - 1] == '\n'))
                    {
                        length--;
                    }
                    
                    buffer[length] = 0;
                }
                
                return std::string(buffer.get());
            }
            
            return "";
        }
        
    protected:
        bool initialized;
        
        FILE *target;
        int target_fd;
        int backup_fd;
        int pipe_fd[2];
    };
}
