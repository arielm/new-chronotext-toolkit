#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class FreetypeHelper
{
    FT_Library library;
    
public:
    FreetypeHelper();
    ~FreetypeHelper();
    
    const FT_Library& getLib();
};
