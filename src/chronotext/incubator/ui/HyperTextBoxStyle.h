/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/TextBoxStyle.h"

namespace chronotext
{
    typedef std::shared_ptr<class HyperTextBoxStyle> HyperTextBoxStyleRef;
    
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
        linkHitExtra(8)
        {}
    };
}

namespace chr = chronotext;
