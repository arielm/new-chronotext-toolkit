/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "hb.h"

#include "unicode/unistr.h"

#include <string>

namespace chr
{
    namespace zf
    {
        struct TextRun
        {
            int32_t start;
            int32_t end;
            
            hb_script_t script;
            std::string language;
            hb_direction_t direction;
            int tag;
            
            TextRun() = default;
            
            TextRun(int32_t start, int32_t end, hb_script_t script, const std::string &language, hb_direction_t direction, int tag)
            :
            start(start),
            end(end),
            script(script),
            language(language),
            direction(direction),
            tag(tag)
            {}
            
            void apply(const UnicodeString &text, hb_buffer_t *buffer) const
            {
                hb_buffer_clear_contents(buffer);
                
                hb_buffer_set_script(buffer, script);
                hb_buffer_set_direction(buffer, direction);
                
                if (!language.empty())
                {
                    hb_buffer_set_language(buffer, hb_language_from_string(language.data(), -1));
                }
                
                hb_buffer_add_utf16(buffer, text.getBuffer(), text.length(), start, end - start);
            }
        };
    }
}
