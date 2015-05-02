/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/LineLayout.h"

#include <map>

namespace chr
{
    namespace zf
    {
        class VirtualFont;
        
        class LayoutStore
        {
        public:
            typedef std::tuple<VirtualFont*, hb_direction_t, std::string, std::string> Key;

            std::shared_ptr<LineLayout> get(VirtualFont *font, const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            void clear();
            
        protected:
            std::multimap<std::string, std::shared_ptr<LineLayout>> layouts;
        };
    }
}
