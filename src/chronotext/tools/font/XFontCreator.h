#pragma once

#include "FreetypeHelper.h"

#include <map>
#include <list>

#include "cinder/DataTarget.h"

struct FontDescriptor
{
    ci::fs::path filePath;
    int faceIndex;
    
    FontDescriptor(const ci::fs::path &filePath, int faceIndex = 0)
    :
    filePath(filePath),
    faceIndex(faceIndex)
    {}
};

struct XParams
{
    int unitMargin; // NECESSARY WHEN MIPMAPPING IS USED
    int unitPadding; // AFFECTS THE SIZE OF THE SUB-TEXTURE, USEFUL FOR AVOIDING JAGGED EDGES
    
    int maxAtlasWidth;
    int maxAtlasHeight;
    int atlasPadding;
    
    XParams(int unitMargin = 2, int unitPadding = 2, int maxAtlasWidth = 1024, int maxAtlasHeight = 1024, int atlasPadding = 0)
    :
    unitMargin(unitMargin),
    unitPadding(unitPadding),
    maxAtlasWidth(maxAtlasWidth),
    maxAtlasHeight(maxAtlasHeight),
    atlasPadding(atlasPadding)
    {}
};

struct XGlyph
{
    void *data;
    int dataSize;
    
    int width;
    int height;
    int leftExtent;
    int topExtent;
    float advance;
    
    int atlasX;
    int atlasY;
    
    XGlyph(void *buffer, int width, int height)
    :
    width(width),
    height(height)
    {
        dataSize = width * height;
        data = malloc(dataSize);
        memcpy(data, buffer, dataSize);
    }
    
    ~XGlyph()
    {
        free(data);
    }
    
    static bool compareSize(XGlyph *lhs, XGlyph *rhs)
    {
        return (lhs->dataSize > rhs->dataSize);
    }
};

class XFontCreator
{
    XGlyph* getGlyph(wchar_t c);
    
    bool canDisplay(wchar_t c);
    bool isSpace(wchar_t c);
    
    bool bestFit();
    bool pack(int targetWidth, int targetHeight);
    
public:
    FT_Face face;
    
    float height;
    float ascent;
    float descent;
    float spaceWidth;
    float strikethroughFactor;
    float underlineOffset;
    float underlineThickness;
    
    float size;
    XParams params;
    
    int atlasWidth;
    int atlasHeight;
    
    std::map<wchar_t, XGlyph*> glyphs; // SORTED BY UNICODE CHARACTER (SMALLER FIRST)
    std::list<XGlyph*> ordered; // SORTED BY SIZE (LARGEST FIRST)
    
    XFontCreator(const FT_Library &library, const FontDescriptor &descriptor, float size, const std::wstring &characters, const XParams &params);
    ~XFontCreator();
    
    void writeToFolder(const ci::fs::path &folderPath);
    void write(ci::DataTargetRef target);
};
