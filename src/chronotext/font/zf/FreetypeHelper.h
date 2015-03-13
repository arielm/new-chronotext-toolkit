/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H

namespace chr
{
    namespace zf
    {
        class FreetypeHelper
        {
            FT_Library library;
            
        public:
            FreetypeHelper()
            {
                FT_Init_FreeType(&library);
            }
            
            ~FreetypeHelper()
            {
                FT_Done_FreeType(library);
            }
            
            FT_Library getLib() const
            {
                return library;
            }
        };
    }
}
