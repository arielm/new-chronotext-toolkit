/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/LayoutStore.h"
#include "chronotext/font/zf/VirtualFont.h"

using namespace std;

namespace chr
{
    namespace zf
    {
        shared_ptr<LineLayout> LayoutStore::get(VirtualFont *font, const string &text, const string &langHint, hb_direction_t overallDirection)
        {
            const auto found = layouts.equal_range(text);
            
            for (auto it = found.first; it != found.second; ++it)
            {
                if ((it->second->font == font) && (it->second->overallDirection == overallDirection) && (it->second->langHint == langHint))
                {
                    return it->second;
                }
            }
            
            return layouts.emplace(text, shared_ptr<LineLayout>(font->createLineLayout(text, langHint, overallDirection)))->second;
        }
        
        void LayoutStore::clear()
        {
            layouts.clear();
        }
    }
}
