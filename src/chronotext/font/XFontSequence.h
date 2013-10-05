/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <vector>

namespace chronotext
{
    class XFont;
    
    struct Slot
    {
        int count;
        float *vertices;
        float *coords;
        
        Slot(int dimensions, int slotCapacity)
        {
            vertices = new float[slotCapacity * dimensions * 4];
            coords = new float[slotCapacity * 2 * 4];
        }
        
        ~Slot()
        {
            delete[] vertices;
            delete[] coords;
        }
    };
    
    class XFontSequence
    {
        XFont *font;
        int dimensions;
        int slotCapacity;
        
        int slotIndex;
        std::vector<Slot*> slots;
        
    public:
        ~XFontSequence();
        
        void begin(XFont *font, int dimensions, int slotCapacity);
        void end();
        void flush(float *vertices, float *coords, int count);
        void replay();
        void clear();
    };
}

namespace chr = chronotext;
