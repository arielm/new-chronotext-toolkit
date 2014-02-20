/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/xf/FontSequence.h"
#include "chronotext/font/xf/Font.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace xf
    {
        void FontSequence::begin(bool useColor)
        {
            this->useColor = useColor;
            batches.clear();
        }
        
        void FontSequence::end()
        {}
        
        void FontSequence::addBatch(unique_ptr<GlyphBatch> &&batch)
        {
            batches.emplace_back(forward<unique_ptr<GlyphBatch>>(batch));
        }
        
        void FontSequence::replay(Font &font)
        {
            font.begin(useColor);
            
            for (auto &batch : batches)
            {
                batch->flush(font.getIndices(), useColor);
            }
            
            font.end(useColor);
        }
    }
}
