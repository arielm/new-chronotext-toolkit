#pragma once

#include "chronotext/utils/Utils.h"

#include <string>

struct ExtractedLink
{
    int offset;
    int length;
    std::string url;
    
    ExtractedLink(int offset, int length, std::string url) : offset(offset), length(length), url(url) {}
};

class LinkExtractor
{
public:
    static std::pair<std::vector<ExtractedLink>, std::wstring> extract(const std::wstring &input)
    {
        std::vector<ExtractedLink> links;
        std::wstring output;
        
        size_t pos = 0;
        
        while (true)
        {
            int pos1 = input.find(L"<a href=\"", pos);
            
            if (pos1 != std::wstring::npos)
            {
                int pos2 = pos1 + 9;
                int pos3 = input.find(L"\">", pos2);
                
                int pos4 = pos3 + 2;
                int pos5 = input.find(L"</a>", pos4);
                
                output.append(input, pos, pos1 - pos);
                links.push_back(ExtractedLink(output.size(),pos5 - pos4, wstringToUtf8(input.substr(pos2, pos3 - pos2))));
                output.append(input, pos4, pos5 - pos4);
                
                pos = pos5 + 4;
            }
            else
            {
                output.append(input.begin() + pos, input.end());
                break;
            }
        }
        
        return make_pair(links, output);
    }
};
