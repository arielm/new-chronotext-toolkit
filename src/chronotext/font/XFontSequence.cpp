#include "chronotext/font/XFontSequence.h"
#include "chronotext/font/XFont.h"

using namespace std;

namespace chronotext
{
    XFontSequence::~XFontSequence()
    {
        clear();
    }
    
    void XFontSequence::begin(XFont *font, int dimensions, int slotCapacity)
    {
        this->font = font;
        this->dimensions = dimensions;
        this->slotCapacity = slotCapacity;
        
        slotIndex = 0;
    }
    
    void XFontSequence::end()
    {}
    
    void XFontSequence::flush(GLfloat *vertices, GLfloat *coords, int count)
    {
        Slot *slot;
        
        if (slotIndex + 1 > slots.size())
        {
            slot = new Slot(dimensions, slotCapacity);
            slots.push_back(slot);
        }
        else
        {
            slot = slots[slotIndex];
        }
        
        slot->count = count;
        memcpy(slot->vertices, vertices, count * dimensions * 4 * sizeof(GLfloat));
        memcpy(slot->coords, coords, count * 2 * 4 * sizeof(GLfloat));
        
        slotIndex++;
    }

    void XFontSequence::replay()
    {
        font->begin();
        
        for (vector<Slot*>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        {
            Slot *slot = *it;
            
            glVertexPointer(dimensions, GL_FLOAT, 0, slot->vertices);
            glTexCoordPointer(2, GL_FLOAT, 0, slot->coords);
            glDrawElements(GL_TRIANGLES, slot->count * 6, GL_UNSIGNED_SHORT, font->getIndices());
        }
        
        font->end();
    }
    
    void XFontSequence::clear()
    {
        for (vector<Slot*>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        {
            delete *it;
        }
        
        slots.clear();
    }
}
