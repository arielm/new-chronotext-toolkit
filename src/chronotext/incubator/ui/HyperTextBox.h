/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/TextBox.h"
#include "chronotext/incubator/ui/HyperTextBoxStyle.h"
#include "chronotext/incubator/ui/TouchableLink.h"
#include "chronotext/text/LinkExtractor.h"

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
        
    public:
        class Delegate
        {
        public:
            virtual void linkClicked(const std::string &url) = 0;
        };

        HyperTextBoxStyleRef style;
        Delegate *delegate;
        
        std::vector<ExtractedLink> links;
        
        HyperTextBox();
        HyperTextBox(HyperTextBoxStyleRef style);

        void setLocation(float x, float y);
        void setText(const std::wstring &text);
        void selectLink(int linkIndex);
        void deselectLink();
        
        void layout();
        void draw();
        
        std::vector<Touchable*> getTouchables();
        void touchStateChanged(Touchable *touchable, int nextState, int prevState);
        void touchActionPerformed(Touchable *touchable, int action);
    };
}

namespace chr = chronotext;
