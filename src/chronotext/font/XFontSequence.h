/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <vector>
#include <memory>

namespace chronotext
{
    class XFont;
    
    struct Slot
    {
        int count;
        float *vertices;
        
        Slot(int dimensions, int slotCapacity)
        :
        count(0)
        {
            vertices = new float[slotCapacity * (dimensions * 4 + 2 * 4)];
        }
        
        ~Slot()
        {
            delete[] vertices;
        }
    };
    
    class XFontSequence
    {
    public:
        void begin(XFont *font, int dimensions, int slotCapacity);
        void end();
        void flush(float *vertices, int count);
        void replay();
        void clear();
        
    protected:
        XFont *font;
        int dimensions;
        int slotCapacity;
        
        int slotIndex;
        std::vector<std::unique_ptr<Slot>> slots;
    };
}

namespace chr = chronotext;
