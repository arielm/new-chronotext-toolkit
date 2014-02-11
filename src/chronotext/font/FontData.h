/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <map>
#include <memory>

struct FontData
{
    int glyphCount;
    std::map<wchar_t, int> glyphs;

    float nativeFontSize;
    float height;
    float ascent;
    float descent;
    float spaceAdvance;
    float strikethroughFactor;
    float underlineOffset;
    float lineThickness;
    
    float *w;
    float *h;
    float *le;
    float *te;
    float *advance;
    
    float *u1;
    float *v1;
    float *u2;
    float *v2;
    
    FontData(int glyphCount)
    :
    glyphCount(glyphCount)
    {
        w = new float[glyphCount];
        h = new float[glyphCount];
        le = new float[glyphCount];
        te = new float[glyphCount];
        advance = new float[glyphCount];
        
        u1 = new float[glyphCount];
        v1 = new float[glyphCount];
        u2 = new float[glyphCount];
        v2 = new float[glyphCount];
    }
    
    ~FontData()
    {
        delete[] w;
        delete[] h;
        delete[] le;
        delete[] te;
        delete[] advance;
        
        delete[] u1;
        delete[] v1;
        delete[] u2;
        delete[] v2;
    }
};

struct FontAtlas
{
    int width;
    int height;
    unsigned char *data;
    
    FontAtlas(int width, int height)
    :
    width(width),
    height(height)
    {
        data = new unsigned char[width * height](); // ZERO-FILLED
    }
    
    ~FontAtlas()
    {
        delete[] data;
    }
};
