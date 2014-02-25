/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Button.h"
#include "chronotext/font/xf/TextHelper.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr::xf;

namespace chronotext
{
    Button::Button(shared_ptr<Style> style)
    :
    Shape(style),
    Touchable(this, TYPE_CLICKABLE, style->hitExtra),
    style(style),
    delegate(NULL)
    {}

    float Button::getWidth()
    {
        if (autoWidth)
        {
            return paddingLeft + getContentWidth() + paddingRight;
        }
        else
        {
            return width;
        }
    }
    
    float Button::getHeight()
    {
        if (autoHeight)
        {
            return paddingTop + getContentHeight() + paddingBottom;
        }
        else
        {
            return height;
        }
    }
   
    vector<Touchable*> Button::getTouchables()
    {
        vector<Touchable*> touchables;
        touchables.push_back(this);
        return touchables;
    }
    
    Rectf Button::getTouchableBounds()
    {
        return getBounds();
    }
    
    void Button::draw()
    {
        Rectf bounds = getBounds();
        float inset = 0;
        
        if (style->borderColor[state])
        {
            inset = -1;
            
            gl::color(*style->borderColor[state]);
            gl::drawSolidRect(bounds);
        }
        
        if (style->backgroundColor[state])
        {
            gl::color(*style->backgroundColor[state]);
            gl::drawSolidRect(bounds.inflated(Vec2f(inset, inset)));
        }
        
        // ---
        
        if (icon)
        {
            gl::color(*style->color[state]);
            
            glPushMatrix();
            gl::translate(bounds.getCenter());
            gl::scale(icon->scale);
            
            icon->texture->begin();
            icon->texture->drawFromCenter();
            icon->texture->end();
            
            glPopMatrix();
        }
        else if (style->font)
        {
            style->font->setColor(*style->color[state]);
            style->font->setSize(style->fontSize);
            TextHelper::drawTextInRect(*style->font, text, bounds);
            
            if (style->strikethrough[state])
            {
                gl::color(*style->color[state]);
                drawStrikethroughInRect(*style->font, text, bounds);
            }
        }
    }
    
    void Button::touchStateChanged(Touchable *touchable, int nextState, int prevState)
    {}
    
    void Button::touchActionPerformed(Touchable *touchable, int action)
    {
        if (delegate)
        {
            if (action == Touchable::ACTION_CLICKED)
            {
                delegate->buttonClicked(tag);
            }
            else if (action == Touchable::ACTION_TOGGLED)
            {
                delegate->buttonToggled(tag);
            }
        }
    }
    
    float Button::getContentWidth()
    {
        if (icon)
        {
            return icon->getWidth();
        }
        else if (style->font)
        {
            style->font->setSize(style->fontSize);
            return style->font->getStringAdvance(text);
        }
        else
        {
            return 0;
        }
    }
    
    float Button::getContentHeight()
    {
        if (icon)
        {
            return icon->getHeight();
        }
        else if (style->font)
        {
            style->font->setSize(style->fontSize);
            return style->font->getHeight();
        }
        else
        {
            return 0;
        }
    }
    
    void Button::drawStrikethroughInRect(XFont &font, const wstring &text, const Rectf &rect)
    {
        float w = font.getStringAdvance(text);
        float x3 = rect.x1 + (rect.getWidth() - w) * 0.5f;
        float x4 = x3 + w;
        
        float y3 = rect.y1 + rect.getHeight() * 0.5f;
        
        const float vertices[] =
        {
            x3, y3,
            x4, y3
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
