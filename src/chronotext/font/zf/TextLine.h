/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/TextRun.h"

#include <vector>

namespace chronotext
{
    namespace zf
    {
        struct TextLine
        {
            UnicodeString text;
            std::string langHint;
            hb_direction_t overallDirection;
            std::vector<TextRun> runs;
            
            TextLine(const std::string &input, const std::string &langHint, hb_direction_t overallDirection = HB_DIRECTION_INVALID)
            :
            langHint(langHint),
            overallDirection(overallDirection)
            {
                text = UnicodeString::fromUTF8(input);
            }
            
            void addRun(int32_t start, int32_t end, hb_script_t script, const std::string &lang, hb_direction_t direction)
            {
                runs.emplace_back(start, end, script, lang, direction);
            }
        };
    }
}

namespace chr = chronotext;
