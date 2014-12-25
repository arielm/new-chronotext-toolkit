/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadBatchMap.h"
#include "chronotext/font/zf/FontTexture.h"

namespace chr
{
    namespace zf
    {
        class FontSequence
        {
        public:
            bool useColor;
            float anisotropy;
            
            FontSequence() {}
            FontSequence(const FontSequence &other) = delete; // FontSequence CAN'T BE COPIED (I.E. BECAUSE OF THE vector OF unique_ptr)
            
        protected:
            std::vector<std::unique_ptr<QuadBatchMap<FontTexture>>> maps;
            
            void begin(bool useColor = false, float anisotropy = 0);
            void end();
            
            void addMap(std::unique_ptr<QuadBatchMap<FontTexture>> &&map);
            void replay(const uint16_t *indices);
            
            friend class VirtualFont;
        };
    }
}
