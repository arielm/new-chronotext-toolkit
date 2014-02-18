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

#include "cinder/Color.h"

namespace chronotext
{
    namespace xf
    {
        class Font;
        
        struct Slot
        {
            int count;
            float *vertices;
            ci::ColorA *colors;
            
            Slot(int slotCapacity, bool useColor = false)
            :
            count(0),
            colors(NULL)
            {
                vertices = new float[slotCapacity * (3 + 2) * 4];
                
                if (useColor)
                {
                    colors = new ci::ColorA[slotCapacity * 4];
                }
            }
            
            ~Slot()
            {
                delete[] vertices;
                
                if (colors)
                {
                    delete[] colors;
                }
            }
        };
        
        class FontSequence
        {
        public:
            void begin(Font *font, int slotCapacity, bool useColor = false);
            void end();
            void flush(int count, float *vertices, ci::ColorA *colors = NULL);
            void replay();
            void clear();
            
        protected:
            Font *font;
            int slotCapacity;
            bool useColor;
            
            int slotIndex;
            std::vector<std::unique_ptr<Slot>> slots;
        };
    }
}

namespace chr = chronotext;
