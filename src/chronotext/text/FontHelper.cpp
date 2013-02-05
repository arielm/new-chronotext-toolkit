#include "chronotext/text/FontHelper.h"

using namespace std;
using namespace ci;
using namespace chr;

float FontHelper::getStringWidth(XFont *font, const wstring &text, bool snap)
{
    if (snap)
    {
        float w = 0;
        
        for (wstring::const_iterator it = text.begin(); it != text.end(); ++it)
        {
            int cc = font->lookup(*it);
            w += math<float>::floor(font->getGlyphWidth(cc));
        }
        
        return w;
    }
    else
    {
        return font->getStringWidth(text);
    }
}

void FontHelper::drawText(XFont *font, XFontSequence *sequence, const wstring &text, float x, float y, bool snap)
{
    if (snap)
    {
        x = math<float>::floor(x);
        y = math<float>::floor(y);
    }
    
    font->beginSequence(sequence, 2);
    
    for (wstring::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        int cc = font->lookup(*it);
        font->addGlyph(cc, x, y);
        
        float w = font->getGlyphWidth(cc);
        x += snap ? math<float>::floor(w) : w;
    }
    
    font->endSequence();
}

void FontHelper::drawAlignedText(XFont *font, XFontSequence *sequence, const wstring &text, float x, float y, int alignX, int alignY, bool snap)
{
    switch (alignX)
    {
        case ALIGN_MIDDLE:
            x -= getStringWidth(font, text, snap) * 0.5f;
            break;
            
        case ALIGN_RIGHT:
            x -= getStringWidth(font, text, snap);
            break;
    }
    
    switch (alignY)
    {
        case ALIGN_TOP:
            y += font->getDescent();
            break;
            
        case ALIGN_MIDDLE:
            y -= font->getStrikethroughOffset();
            break;
    }
    
    drawText(font, sequence, text, x, y, snap);
}

void FontHelper::drawTextInRect(XFont *font, XFontSequence *sequence, const wstring &text, const Rectf &rect, bool snap)
{
    drawTextInRect(font, sequence, text, rect.x1, rect.y1, rect.x2, rect.y2, snap);
}

void FontHelper::drawTextInRect(XFont *font, XFontSequence *sequence, const wstring &text, float x1, float y1, float x2, float y2, bool snap)
{
    float w = x2 - x1;
    float x = x1 + (w - getStringWidth(font, text, snap)) * 0.5f;
    
    float h = y2 - y1;
    float y = y1 + h * 0.5f - font->getStrikethroughOffset();
    
    drawText(font, sequence, text, x, y, snap);
}

void FontHelper::drawStrikethroughInRect(XFont *font, const wstring &text, const Rectf &rect, bool snap)
{
    drawStrikethroughInRect(font, text, rect.x1, rect.y1, rect.x2, rect.y2, snap);
}

void FontHelper::drawStrikethroughInRect(XFont *font, const wstring &text, float x1, float y1, float x2, float y2, bool snap)
{
    float w1 = getStringWidth(font, text, snap);
    float w2 = x2 - x1;
    float x3 = x1 + (w2 - w1) * 0.5f;
    float x4 = x3 + w1;
    
    float h = y2 - y1;
    float y3 = y1 + h * 0.5f;
    
    if (snap)
    {
        x3 = math<float>::floor(x3);
        x4 = math<float>::floor(x4);
        y3 = math<float>::floor(y3);
    }
    
    const GLfloat vertices[] =
    {
        x3, y3,
        x4, y3
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
}
