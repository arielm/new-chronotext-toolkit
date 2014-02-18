/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/TextBox.h"
#include "chronotext/incubator/ui/TouchableLink.h"
#include "chronotext/incubator/ui/LinkExtractor.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace chronotext
{
    typedef std::shared_ptr<class HyperTextBox> HyperTextBoxRef;
    
    struct LinkSpan
    {
        int startIndex;
        int endIndex;
        
        int startLine;
        int endLine;
        
        std::vector< std::pair<ci::Vec2f, ci::Vec2f> > positions;
        
        LinkSpan(int startIndex, int endIndex, int startLine, int endLine)
        :
        startIndex(startIndex),
        endIndex(endIndex),
        startLine(startLine),
        endLine(endLine)
        {}
    };
    
    class HyperTextBox : public TextBox
    {
    public:
        class Delegate
        {
        public:
            virtual void linkClicked(const std::string &url) = 0;
        };
        
        class Style : public TextBox::Style
        {
        public:
            ci::ColorA selectedLinkTextColor;
            ci::ColorA selectedLinkBackgroundColor;
            
            float linkUnderlineFactor;
            float linkPaddingFactor;
            float linkHitExtra;
            
            Style()
            :
            TextBox::Style(),
            linkUnderlineFactor(0.2),
            linkPaddingFactor(0.1),
            linkHitExtra(8)
            {}
        };

        Delegate *delegate;
        std::shared_ptr<Style> style;
        
        std::vector<ExtractedLink> links;
        
        HyperTextBox();
        HyperTextBox(std::shared_ptr<Style> style);

        void setLocation(float x, float y);
        void setText(const std::wstring &text);
        void selectLink(int linkIndex);
        void deselectLink();
        
        void layout();
        void draw();
        
        std::vector<Touchable*> getTouchables();
        void touchStateChanged(Touchable *touchable, int nextState, int prevState);
        void touchActionPerformed(Touchable *touchable, int action);
        
    protected:
        bool updateLocationRequest;
        
        int selectedLinkIndex;
        std::vector<LinkSpan> linkSpans;
        boost::ptr_vector<Touchable> touchableLinks;
        
        LinkSpan getLinkSpan(const ExtractedLink &link);
        
        void drawText();
        void drawSelectedLinkText();
        void drawSelectedLinkBackground();
        void drawLinkUnderlines();
    };
}

namespace chr = chronotext;
