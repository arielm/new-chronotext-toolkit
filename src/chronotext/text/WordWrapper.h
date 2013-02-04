#pragma once

#include "chronotext/font/XFont.h"

#include <vector>

class WordWrapper
{
    void addLine(const std::wstring &text, int offset, int length);
    
public:
    int size;
    std::vector<int> offsets;
    std::vector<int> lengths;
    
    float wrap(chr::XFont *font, const std::wstring &text);
    void wrap(chr::XFont *font, const std::wstring &text, float width);

    int getLine(int offset);
};
