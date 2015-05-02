/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FontSequence.h"

using namespace std;

namespace chr
{
    namespace xf
    {
        void FontSequence::begin(bool useColor)
        {
            FontSequence::useColor = useColor;
            batches.clear();
        }
        
        void FontSequence::end()
        {}
        
        void FontSequence::addBatch(unique_ptr<QuadBatch> &&batch)
        {
            batches.emplace_back(forward<unique_ptr<QuadBatch>>(batch));
        }
        
        void FontSequence::replay(const uint16_t *indices)
        {
            for (auto &batch : batches)
            {
                batch->flush(indices, useColor);
            }
        }
    }
}
