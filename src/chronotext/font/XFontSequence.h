#pragma once

#include "cinder/gl/gl.h"

namespace chronotext
{
    struct Slot
    {
        int count;
        GLuint verticesName;
        GLuint coordsName;
        
        Slot(int count) : count(count) {}
        
        ~Slot()
        {
            glDeleteBuffers(1, &verticesName);
            glDeleteBuffers(1, &coordsName);
        }
    };
    
    class XFont;
    
    class XFontSequence
    {
        XFont *font;
        int dimensions;
        std::list<Slot*> slots;
        
    public:
        ~XFontSequence();
        
        void begin(XFont *font, int dimensions);
        void end();
        void flush(GLfloat *vertices, GLfloat *coords, int count);
        void replay();
        void clear();
    };
}

namespace chr = chronotext;
