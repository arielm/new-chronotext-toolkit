/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadBatch.h"

namespace chr
{
    namespace xf
    {
        class Font;
        
        class FontSequence
        {
        public:
            bool useColor;
            
            FontSequence() = default;
            
            /*
             * NOT ALLOWING COPY, BECAUSE OF THE vector OF unique_ptr
             */
            FontSequence(const FontSequence &other) = delete;
            void operator=(const FontSequence &other) = delete;
            
        protected:
            std::vector<std::unique_ptr<QuadBatch>> batches;
            
            void begin(bool useColor = false);
            void end();
            
            void addBatch(std::unique_ptr<QuadBatch> &&batch);
            void replay(const uint16_t *indices);
            
            friend class Font;
        };
    }
}
