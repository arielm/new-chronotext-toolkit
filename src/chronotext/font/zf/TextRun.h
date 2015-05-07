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

namespace chr
{
    namespace zf
    {
        struct TextRun
        {
            int32_t start = 0;
            int32_t end = 0;
            
            hb_script_t script = HB_SCRIPT_INVALID;
            hb_language_t language = HB_LANGUAGE_INVALID;
            hb_direction_t direction = HB_DIRECTION_INVALID;
            int tag = -1;
            
            TextRun() = default;
            
            TextRun(int32_t start, int32_t end, hb_script_t script, hb_language_t language, hb_direction_t direction, int tag)
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
                hb_buffer_set_language(buffer, language);
                hb_buffer_set_direction(buffer, direction);
                
                hb_buffer_add_utf16(buffer, text.getBuffer(), text.length(), start, end - start);
            }
        };
    }
}
