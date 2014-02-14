/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/XFontSequence.h"
#include "chronotext/font/XFont.h"

#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    void XFontSequence::begin(XFont *font, int slotCapacity, int dimensions, bool useColor)
    {
        this->font = font;
        this->slotCapacity = slotCapacity;
        this->dimensions = dimensions;
        this->useColor = useColor;
        
        slotIndex = 0;
    }
    
    void XFontSequence::end()
    {}
    
    void XFontSequence::flush(int count, float *vertices, ci::ColorA *colors)
    {
        assert(!useColor || colors);
        
        // ---
        
        Slot *slot;
        
        if (slotIndex + 1 > slots.size())
        {
            slot = new Slot(slotCapacity, dimensions, useColor);
            slots.push_back(unique_ptr<Slot>(slot));
        }
        else
        {
            slot = slots[slotIndex].get();
        }
        
        memcpy(slot->vertices, vertices, sizeof(float) * count * (dimensions + 2) * 4);
        
        if (useColor)
        {
            memcpy(slot->colors, colors, sizeof(ColorA) * count * 4);
        }
        
        slot->count = count;
        
        // ---
        
        slotIndex++;
    }

    void XFontSequence::replay()
    {
        font->begin(useColor);
        
        for (auto &slot : slots)
        {
            int stride = sizeof(float) * (dimensions + 2);

            if (useColor)
            {
                glColorPointer(4, GL_FLOAT, 0, slot->colors);
            }

            glVertexPointer(dimensions, GL_FLOAT, stride, slot->vertices);
            glTexCoordPointer(2, GL_FLOAT, stride, slot->vertices + dimensions);
            glDrawElements(GL_TRIANGLES, slot->count * 6, GL_UNSIGNED_SHORT, font->getIndices());
        }
        
        font->end(useColor);
    }
    
    void XFontSequence::clear()
    {
        slots.clear();
    }
}
