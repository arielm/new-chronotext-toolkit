#include "chronotext/font/XFontSequence.h"
#include "chronotext/font/XFont.h"

using namespace std;

namespace chronotext
{
    XFontSequence::~XFontSequence()
    {
        clear();
    }
    
    void XFontSequence::begin(XFont *font, int dimensions)
    {
        this->font = font;
        this->dimensions = dimensions;
    }
    
    void XFontSequence::end()
    {}
    
    void XFontSequence::flush(GLfloat *vertices, GLfloat *coords, int count)
    {
        Slot *slot = new Slot(count);
        slots.push_back(slot);
        
        glGenBuffers(1, &slot->verticesName);
        glBindBuffer(GL_ARRAY_BUFFER, slot->verticesName);
        glBufferData(GL_ARRAY_BUFFER, count * dimensions * 4 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &slot->coordsName);
        glBindBuffer(GL_ARRAY_BUFFER, slot->coordsName);
        glBufferData(GL_ARRAY_BUFFER, count * 2 * 4 * sizeof(GLfloat), coords, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void XFontSequence::replay()
    {
        font->begin();
        
        for (list<Slot*>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        {
            Slot *slot = *it;
            
            glBindBuffer(GL_ARRAY_BUFFER, slot->verticesName);
            glVertexPointer(dimensions, GL_FLOAT, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, slot->coordsName);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->getIndicesName());
            glDrawElements(GL_TRIANGLES, slot->count * 6, GL_UNSIGNED_SHORT, 0);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        font->end();
    }
    
    void XFontSequence::clear()
    {
        for (list<Slot*>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        {
            delete *it;
        }
        
        slots.clear();
    }
}
