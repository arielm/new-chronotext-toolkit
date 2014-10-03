/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadBatch.h"

namespace chronotext
{
    namespace xf
    {
        class Font;
        
        class FontSequence
        {
        public:
            bool useColor;
            
            FontSequence() {}
            
        protected:
            std::vector<std::unique_ptr<QuadBatch>> batches;
            
            void begin(bool useColor = false);
            void end();
            
            void addBatch(std::unique_ptr<QuadBatch> &&batch);
            void replay(const uint16_t *indices);
            
            friend class Font;
            
		private:
            FontSequence(const FontSequence &that); // MAKES IT EXPLICIT: FontSequence CAN'T BE COPIED (I.E. BECAUSE OF THE vector OF unique_ptr)
        };
    }
}

namespace chr = chronotext;
