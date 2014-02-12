/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/XFont.h"

#include <vector>

namespace chronotext
{
    class WordWrapper
    {
        void addLine(const std::wstring &text, int offset, int length);
        
    public:
        int size;
        std::vector<int> offsets;
        std::vector<int> lengths;
        
        float wrap(const XFont &font, const std::wstring &text);
        void wrap(const XFont &font, const std::wstring &text, float width);
        
        int getLine(int offset);
    };
}

namespace chr = chronotext;
