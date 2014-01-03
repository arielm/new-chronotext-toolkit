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

namespace chronotext
{
    void XFontSequence::begin(XFont *font, int dimensions, int slotCapacity)
    {
        this->font = font;
        this->dimensions = dimensions;
        this->slotCapacity = slotCapacity;
        
        slotIndex = 0;
    }
    
    void XFontSequence::end()
    {}
    
    void XFontSequence::flush(float *vertices, float *coords, int count)
    {
        Slot *slot;
        
        if (slotIndex + 1 > slots.size())
        {
            slot = new Slot(dimensions, slotCapacity);
            slots.push_back(unique_ptr<Slot>(slot));
        }
        else
        {
            slot = slots[slotIndex].get();
        }
        
        slot->count = count;
        memcpy(slot->vertices, vertices, count * dimensions * 4 * sizeof(float));
        memcpy(slot->coords, coords, count * 2 * 4 * sizeof(float));
        
        slotIndex++;
    }

    void XFontSequence::replay()
    {
        font->begin();
        
        for (auto &slot : slots)
        {
            glVertexPointer(dimensions, GL_FLOAT, 0, slot->vertices);
            glTexCoordPointer(2, GL_FLOAT, 0, slot->coords);
            glDrawElements(GL_TRIANGLES, slot->count * 6, GL_UNSIGNED_SHORT, font->getIndices());
        }
        
        font->end();
    }
    
    void XFontSequence::clear()
    {
        slots.clear();
    }
}
