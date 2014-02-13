/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/incubator/ui/Icon.h"
#include "chronotext/incubator/ui/Touchable.h"

#include "chronotext/font/XFont.h"

#include <map>

namespace chronotext
{
    typedef std::shared_ptr<class Button> ButtonRef;
    
    class Button : public Shape, public Touchable
    {
    public:
        class Delegate
        {
        public:
            virtual void buttonClicked(int tag) = 0;
            virtual void buttonToggled(int tag) = 0;
        };
        
        class Style : public Shape::Style
        {
        public:
            std::shared_ptr<chr::XFont> font;
            float fontSize;
            bool snap;
            
            std::map<int, std::shared_ptr<ci::ColorA>> borderColor;
            std::map<int, std::shared_ptr<ci::ColorA>> backgroundColor;
            std::map<int, std::shared_ptr<ci::ColorA>> color;
            
            std::map<int, bool> strikethrough;
            
            float hitExtra;
            
            Style()
            :
            Shape::Style(),
            fontSize(1),
            snap(false),
            hitExtra(8)
            {}
        };

        Delegate *delegate;
        std::shared_ptr<Style> style;
        
        std::wstring text;
        IconRef icon;
        
        Button(std::shared_ptr<Style> style);
        
        float getWidth();
        float getHeight();
        
        std::vector<Touchable*> getTouchables();
        ci::Rectf getTouchableBounds();
        
        void draw();
        
        void touchStateChanged(Touchable *touchable, int nextState, int prevState);
        void touchActionPerformed(Touchable *touchable, int action);
        
    protected:
        float getContentWidth();
        float getContentHeight();
    };
}

namespace chr = chronotext;
