/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FreetypeHelper.h"

FreetypeHelper::FreetypeHelper()
{
    FT_Init_FreeType(&library);
}

FreetypeHelper::~FreetypeHelper()
{
    FT_Done_FreeType(library);
}

const FT_Library& FreetypeHelper::getLib()
{
    return library;
}
