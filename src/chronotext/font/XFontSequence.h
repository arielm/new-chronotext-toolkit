#pragma once

#include "cinder/gl/gl.h"

namespace chronotext
{
    class XFont;
    
    struct Slot
    {
        int count;
        GLfloat *vertices;
        GLfloat *coords;
        
        Slot(int dimensions, int slotCapacity)
        {
            vertices = new GLfloat[slotCapacity * dimensions * 4];
            coords = new GLfloat[slotCapacity * 2 * 4];
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
        void flush(GLfloat *vertices, GLfloat *coords, int count);
        void replay();
        void clear();
    };
}

namespace chr = chronotext;
