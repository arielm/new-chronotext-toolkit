/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "XFontCreator.h"
#include "BinPack.h"

#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace chr;

XFontCreator::XFontCreator(FT_Library library, const FontDescriptor &descriptor, float size, const wstring &characters, const XParams &params)
:
size(size),
params(params)
{
    FT_Error error = FT_New_Face(library, descriptor.filePath.string().c_str(), descriptor.faceIndex, &face);
    
    if (error == FT_Err_Unknown_File_Format)
    {
        throw runtime_error("FREETYPE ERROR: UNKNOWN FILE FORMAT - " + descriptor.filePath.string());
    }
    else if (error)
    {
        throw runtime_error("FREETYPE ERROR: CAN'T OPEN FILE - " + descriptor.filePath.string());
    }
    
    // ---
    
    /*
     * USING A MATRIX WITH A MULTIPLIER ALLOWS FOR FRACTIONAL VALUES
     * TRICK FROM http://code.google.com/p/freetype-gl/
     *
     * - WITHOUT A FRACTIONAL ADVANCE: CHARACTER SPACING LOOKS DUMB
     * - WITHOUT A FRACTIONAL HEIGHT: SOME CHARACTERS WON'T BE PROPERLY ALIGNED ON THE BASE-LINE
     */
    int res = 64;
    int dpi = 72;
    
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Char_Size(face, size * 64, 0, dpi * res, dpi * res);
    
    FT_Matrix matrix =
    {
        int((1.0 / res) * 0x10000L),
        int((0.0) * 0x10000L),
        int((0.0) * 0x10000L),
        int((1.0 / res) * 0x10000L)
    };
    
    FT_Set_Transform(face, &matrix, NULL);
    
    // ---
    
    height = face->size->metrics.height / 64.0f / res;
    ascent = face->size->metrics.ascender / 64.0f / res;
    descent = -face->size->metrics.descender / 64.0f / res;

    underlineOffset = -face->underline_position / 64.0f;
    underlineThickness = face->underline_thickness / 64.0f;

    // ---
    
    FT_UInt spaceGlyphIndex = FT_Get_Char_Index(face, L' ');
    FT_Load_Glyph(face, spaceGlyphIndex, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT);
    spaceWidth = face->glyph->advance.x / 64.0f;
    
    // ----

    FT_UInt minusGlyphIndex = FT_Get_Char_Index(face, L'-');
    FT_Load_Glyph(face, minusGlyphIndex, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT);
    float minusHeight = face->glyph->metrics.height / 64.0f / res;
    float minusBearingY = face->glyph->metrics.horiBearingY / 64.0f / res;
    float strikethroughOffset = minusBearingY - minusHeight * 0.5f;
    strikethroughFactor = strikethroughOffset / (ascent - descent);

    // ---
    
    for (wstring::const_iterator it = characters.begin(); it != characters.end(); ++it)
    {
        if (!isSpace(*it) && canDisplay(*it))
        {
            if (glyphs.find(*it) == glyphs.end())
            {
                XGlyph *glyph = getGlyph(*it);
                
                if (glyph)
                {
                    glyphs[*it] = glyph;
                    ordered.push_back(glyph);
                }
            }
        }
    }
    
    ordered.sort(XGlyph::compareSize);
    
    if (!bestFit())
    {
        throw runtime_error("FONT CREATION: ATLAS OVERFLOW");
    }
}

XFontCreator::~XFontCreator()
{
    for (map<wchar_t, XGlyph*>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it)
    {
        delete it->second;
    }

    FT_Done_Face(face);
}

void XFontCreator::writeToFolder(const fs::path &folderPath)
{
    string fileName = string(face->family_name) + "_" + string(face->style_name) + "_" + boost::lexical_cast<string>(size) + ".fnt";
    write(writeFile(folderPath / fileName));
    
    LOGI << "GENERATED: " << fileName << " - " << atlasWidth << "x" << atlasHeight << endl;
}

void XFontCreator::write(DataTargetRef target)
{
    OStreamRef out = target->getStream();
    
    string version = "XFONT.003";
    out->write(version);
    
    int glyphCount = glyphs.size();
    out->writeLittle(glyphCount);
    
    out->writeLittle(size);
    out->writeLittle(height);
    out->writeLittle(ascent);
    out->writeLittle(descent);
    out->writeLittle(spaceWidth);
    out->writeLittle(strikethroughFactor);
    out->writeLittle(underlineOffset);
    out->writeLittle(underlineThickness);
    
    out->writeLittle(atlasWidth);
    out->writeLittle(atlasHeight);
    out->writeLittle(params.atlasPadding);
    out->writeLittle(params.unitMargin);
    out->writeLittle(params.unitPadding);
    
    for (map<wchar_t, XGlyph*>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it)
    {
        out->writeLittle((int)it->first);
        
        XGlyph *glyph = it->second;
        out->writeLittle(glyph->advance);
        out->writeLittle(glyph->width);
        out->writeLittle(glyph->height);
        out->writeLittle(glyph->leftExtent);
        out->writeLittle(glyph->topExtent);
        out->writeLittle(glyph->atlasX);
        out->writeLittle(glyph->atlasY);
        out->writeData(glyph->data, glyph->dataSize);
    }
}

XGlyph* XFontCreator::getGlyph(wchar_t c)
{
    FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
    
    if (index)
    {
        FT_Error error =  FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT);
        
        if (!error)
        {
            FT_GlyphSlot slot = face->glyph;
            
            FT_Glyph glyph;
            error = FT_Get_Glyph(slot, &glyph);
            
            if (!error)
            {
                FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
                
                /*
                 * WE CAN'T TRUST THE FACT THAT FT_Get_Glyph()
                 * IS NOT RETURNING AN ERROR
                 */
                if (slot->bitmap.width * slot->bitmap.rows > 0)
                {
                    XGlyph *g = new XGlyph(slot->bitmap.buffer, slot->bitmap.width, slot->bitmap.rows);
                    g->leftExtent = slot->bitmap_left;
                    g->topExtent = slot->bitmap_top;
                    g->advance = slot->advance.x / 64.0f;
                    
                    FT_Done_Glyph(glyph);
                    return g;
                }
            }
        }
    }
    
    return NULL;
}

bool XFontCreator::canDisplay(wchar_t c)
{
    return (c > 0) && FT_Get_Char_Index(face, c);
}

bool XFontCreator::isSpace(wchar_t c)
{
    return (c == 0x20 || c == 0xa0);
}

/*
 * VERY BASIC SYSTEM FOR AUTOMATICALLY DEFINING THE
 * DIMENSION OF THE ATLAS BASED ON CONTENT
 *
 * THE CURRENT SYSTEM GIVES "GROW-PRIORITY"
 * TO THE VERTICAL AXIS
 */
bool XFontCreator::bestFit()
{
    int minW = 16;
    int minH = 16;
    
    int maxW = params.maxAtlasWidth;
    int maxH = params.maxAtlasHeight;
    
    int bestW = minW;
    int bestH = minH;
    while (bestW <= maxW)
    {
        bestH = minH;
        while (bestH <= maxH)
        {
            if (pack(bestW, bestH))
            {
                atlasWidth = bestW;
                atlasHeight = bestH;
                return true;
            }
            
            if (bestH == maxH) break;
            bestH <<= 1;
        }
        if (bestW == maxW) break;
        bestW <<= 1;
    }
    
    atlasWidth = bestW;
    atlasHeight = bestH;
    return pack(bestW, bestH);
}

bool XFontCreator::pack(int targetWidth, int targetHeight)
{
    int atlasPaddingExtra = 2 * params.atlasPadding;
    int unitMarginExtra = 2 * params.unitMargin;
    
    BinPackRef pack = BinPack::create(targetWidth - atlasPaddingExtra, targetHeight - atlasPaddingExtra, BinPack::SKYLINE);
    
    for (list<XGlyph*>::const_iterator it = ordered.begin(); it != ordered.end(); ++it)
    {
        XGlyph *glyph = *it;
        Area area = pack->allocateArea(glyph->width + unitMarginExtra, glyph->height + unitMarginExtra);
        
        if (area.x1 < 0)
        {
            return false;
        }
        else
        {
            glyph->atlasX = area.x1;
            glyph->atlasY = area.y1;
        }
    }
    
    return true;
}
