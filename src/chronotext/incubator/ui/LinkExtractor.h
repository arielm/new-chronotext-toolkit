/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/utils/Utils.h"

namespace chr
{
    struct ExtractedLink
    {
        int offset;
        int length;
        std::string url;
        
        ExtractedLink(int offset, int length, std::string url)
        :
        offset(offset),
        length(length),
        url(url)
        {}
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
                    links.emplace_back(output.size(), pos5 - pos4, utils::to<std::string>(input.substr(pos2, pos3 - pos2)));
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
}
