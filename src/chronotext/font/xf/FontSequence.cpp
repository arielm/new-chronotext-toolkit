/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/xf/FontSequence.h"
#include "chronotext/font/xf/Font.h"

#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace xf
    {
        void FontSequence::begin(Font *font, int slotCapacity, bool useColor)
        {
            this->font = font;
            this->slotCapacity = slotCapacity;
            this->useColor = useColor;
            
            slotIndex = 0;
        }
        
        void FontSequence::end()
        {}
        
        void FontSequence::flush(int count, float *vertices, ci::ColorA *colors)
        {
            assert(!useColor || colors);
            
            // ---
            
            Slot *slot;
            
            if (slotIndex + 1 > slots.size())
            {
                slot = new Slot(slotCapacity, useColor);
                slots.push_back(unique_ptr<Slot>(slot));
            }
            else
            {
                slot = slots[slotIndex].get();
            }
            
            memcpy(slot->vertices, vertices, sizeof(float) * count * (3 + 2) * 4);
            
            if (useColor)
            {
                memcpy(slot->colors, colors, sizeof(ColorA) * count * 4);
            }
            
            slot->count = count;
            slotIndex++;
        }
        
        void FontSequence::replay()
        {
            font->begin(useColor);
            
            for (auto &slot : slots)
            {
                static const int stride = sizeof(float) * (3 + 2);
                
                if (useColor)
                {
                    glColorPointer(4, GL_FLOAT, 0, slot->colors);
                }
                
                glVertexPointer(3, GL_FLOAT, stride, slot->vertices);
                glTexCoordPointer(2, GL_FLOAT, stride, slot->vertices + 3);
                glDrawElements(GL_TRIANGLES, slot->count * 6, GL_UNSIGNED_SHORT, font->getIndices());
            }
            
            font->end(useColor);
        }
        
        void FontSequence::clear()
        {
            slots.clear();
        }
    }
}
