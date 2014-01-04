/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H

class FreetypeHelper
{
    FT_Library library;
    
public:
    FreetypeHelper();
    ~FreetypeHelper();
    
    FT_Library getLib() const;
};
