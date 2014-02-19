/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/GlyphBatch.h"

namespace chronotext
{
    namespace xf
    {
        class Font;
        
        class FontSequence
        {
        public:
            void begin(bool useColor = false);
            void end();
            
            void addBatch(const GlyphBatch &batch);
            void replay(Font &font);
            
        protected:
            bool useColor;
            std::vector<GlyphBatch> batches;
        };
    }
}

namespace chr = chronotext;
