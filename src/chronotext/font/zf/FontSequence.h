/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/GlyphBatchMap.h"

namespace chronotext
{
    namespace zf
    {
        class FontSequence
        {
        public:
            bool useColor;
            float anisotropy;

        protected:
            std::vector<std::unique_ptr<GlyphBatchMap>> maps;
            
            void begin(bool useColor = false, float anisotropy = 1);
            void end();
            
            void addMap(std::unique_ptr<GlyphBatchMap> &&map);
            void replay(const GLushort *indices);
            
            friend class VirtualFont;
        };
    }
}

namespace chr = chronotext;
