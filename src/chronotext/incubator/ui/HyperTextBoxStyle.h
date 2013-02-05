#pragma once

#include "chronotext/incubator/ui/TextBoxStyle.h"

namespace chronotext
{
    typedef boost::shared_ptr<class HyperTextBoxStyle> HyperTextBoxStyleRef;
    
    class HyperTextBoxStyle : public TextBoxStyle
    {
    public:
        ci::ColorAf selectedLinkTextColor;
        ci::ColorAf selectedLinkBackgroundColor;
        
        float linkUnderlineFactor;
        float linkPaddingFactor;
        float linkHitExtra;
        
        HyperTextBoxStyle()
        :
        TextBoxStyle(),
        linkUnderlineFactor(0.2),
        linkPaddingFactor(0.1),
        linkHitExtra(8)
        {}
    };
}

namespace chr = chronotext;
