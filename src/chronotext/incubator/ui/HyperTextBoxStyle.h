#pragma once

#include "chronotext/incubator/ui/TextBoxStyle.h"

namespace chronotext
{
    typedef boost::shared_ptr<class HyperTextBoxStyle> HyperTextBoxStyleRef;
    
    class HyperTextBoxStyle : public TextBoxStyle
    {
    public:
        ci::ColorA selectedLinkTextColor;
        ci::ColorA selectedLinkBackgroundColor;
        
        float linkUnderlineFactor;
        float linkPaddingFactor;
        float linkHitExtra;
        
        HyperTextBoxStyle()
        :
        TextBoxStyle(),
        linkUnderlineFactor(0.2),
        linkPaddingFactor(0.1),
        linkHitExtra(8),
        selectedLinkBackgroundColor(ci::ColorA(0, 0, 0, 0))
        {}
    };
}

namespace chr = chronotext;
