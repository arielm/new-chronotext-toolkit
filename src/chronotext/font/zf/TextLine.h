/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/TextRun.h"

#include <list>

namespace chr
{
    namespace zf
    {
        struct TextLine
        {
            template<typename T>
            struct Item
            {
                int32_t start;
                int32_t end;
                T data;
                
                Item(int32_t start, int32_t end, T data)
                :
                start(start),
                end(end),
                data(data)
                {}
            };
            
            typedef Item<std::pair<hb_script_t, hb_language_t>> ScriptAndLanguageItem;
            typedef Item<hb_direction_t> DirectionItem;
            typedef Item<int> TagItem;
            
            UnicodeString text;
            hb_language_t langHint;
            hb_direction_t overallDirection;
            
            std::vector<ScriptAndLanguageItem> scriptAndLanguageItems;
            std::vector<DirectionItem> directionItems;
            std::vector<TagItem> tagItems;
            
            std::list<TextRun> runs;
            
            TextLine(hb_language_t langHint = HB_LANGUAGE_INVALID, hb_direction_t overallDirection = HB_DIRECTION_INVALID)
            :
            langHint(langHint),
            overallDirection(overallDirection)
            {}
            
            TextLine(const std::string &input, hb_language_t langHint = HB_LANGUAGE_INVALID, hb_direction_t overallDirection = HB_DIRECTION_INVALID)
            :
            text(input.data()),
            langHint(langHint),
            overallDirection(overallDirection)
            {}
            
            void addChunk(const std::string &input, int tag)
            {
                auto start = text.length();
                text += input.data();
                
                tagItems.emplace_back(start, text.length(), tag);
            }
        };
    }
}
