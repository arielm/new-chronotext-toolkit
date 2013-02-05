#pragma once

#include "chronotext/incubator/ui/TextBox.h"
#include "chronotext/incubator/ui/HyperTextBoxStyle.h"
#include "chronotext/incubator/ui/TouchableLink.h"
#include "chronotext/text/LinkExtractor.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace chronotext
{
    typedef boost::shared_ptr<class HyperTextBox> HyperTextBoxRef;
    
    class HyperTextBoxDelegate
    {
    public:
        virtual void linkClicked(const std::string &url) = 0;
    };
    
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
        int selectedLinkIndex;
        std::vector<LinkSpan> linkSpans;
        boost::ptr_vector<Touchable> touchableLinks;
        
        LinkSpan getLinkSpan(const ExtractedLink &link);
        
        void drawText();
        void drawSelectedLinkText();
        void drawSelectedLinkBackground();
        void drawLinkUnderlines();
        
    public:
        HyperTextBoxStyleRef style;

        HyperTextBoxDelegate *delegate;
        std::vector<ExtractedLink> links;
        
        HyperTextBox() : TextBox() {}
        
        HyperTextBox(HyperTextBoxStyleRef style)
        :
        TextBox(style),
        style(style),
        delegate(NULL),
        selectedLinkIndex(-1)
        {}

        void setText(const std::wstring &text);
        void selectLink(int linkIndex);
        void deselectLink();
        
        void layout();
        void draw();
        
        std::vector<Touchable*> getTouchables();
        void touchStateChanged(Touchable *touchable, int state);
        void touchActionPerformed(Touchable *touchable, int action);
    };
}

namespace chr = chronotext;
