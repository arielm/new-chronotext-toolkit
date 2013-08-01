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
