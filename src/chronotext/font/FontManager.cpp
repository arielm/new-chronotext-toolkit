/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/FontManager.h"
#include "chronotext/utils/Utils.h"

using namespace ci;
using namespace std;
using namespace chr;

namespace chronotext
{
    FontData::FontData(int glyphCount)
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
    
    FontData::~FontData()
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
    
    // ---
    
    FontAtlas::FontAtlas(int width, int height)
    :
    width(width),
    height(height)
    {
        data = new unsigned char[width * height](); // ZERO-FILLED
    }
    
    FontAtlas::~FontAtlas()
    {
        delete[] data;
    }
    
    void FontAtlas::addGlyph(unsigned char *glyphData, int x, int y, int glyphWidth, int glyphHeight)
    {
        for (int iy = 0; iy < glyphHeight; iy++)
        {
            for (int ix = 0; ix < glyphWidth; ix++)
            {
                data[(iy + y) * width + ix + x] = glyphData[iy * glyphWidth + ix];
            }
        }
    }
    
    // ---
    
    FontTexture::FontTexture(FontAtlas *atlas, InputSourceRef inputSource, bool useMipmap)
    :
    width(atlas->width),
    height(atlas->height),
    name(0),
    inputSource(inputSource),
    useMipmap(useMipmap)
    {
        upload(atlas);
    }
    
    FontTexture::~FontTexture()
    {
        discard();
    }
    
    void FontTexture::upload(FontAtlas *atlas)
    {
        if (!name)
        {
            glGenTextures(1, &name);
            glBindTexture(GL_TEXTURE_2D, name);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas->data);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
            }
            
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // ---
            
            LOGD <<
            "FONT UPLOADED: " <<
            inputSource->getFilePathHint() << " | " <<
            name << " | " <<
            width << "x" << height  <<
            (useMipmap ? " (M)" : "") <<
            endl;

        }
    }
    
    void FontTexture::discard()
    {
        if (name)
        {
            LOGD <<
            "FONT DISCARDED: " <<
            name <<
            endl;
            
            // ---
            
            glDeleteTextures(1, &name);
            name = 0;
        }
    }
    
    // ---
    
    XFont* FontManager::getFont(const string &resourceName, const XFont::Properties &properties)
    {
        return getFont(InputSource::getResource(resourceName), properties);
    }
    
    XFont* FontManager::getFont(InputSourceRef inputSource, const XFont::Properties &properties)
    {
        auto uri = inputSource->getURI();
        
        FontKey key(uri, properties.useMipmap, properties.useAnisotropy, properties.maxDimensions, properties.slotCapacity);
        auto it1 = fonts.find(key);
        
        if (it1 != fonts.end())
        {
            return it1->second.get();
        }
        else
        {
            FontData *data;
            auto it2 = fontData.find(uri);
            
            if (it2 == fontData.end())
            {
                FontAtlas *atlas;
                tie(data, atlas) = fetchFont(inputSource); // CAN THROW

                fontData[uri] = unique_ptr<FontData>(data);
                
                auto texture = new FontTexture(atlas, inputSource, properties.useMipmap);
                textures[make_pair(inputSource->getURI(), properties.useMipmap)] = unique_ptr<FontTexture>(texture);
                delete atlas;
            }
            else
            {
                data = it2->second.get();
            }
            
            FontTexture *texture;
            auto it3 = textures.find(make_pair(uri, properties.useMipmap));
            
            if (it3 == textures.end())
            {
                FontData *tmp;
                FontAtlas *atlas;
                tie(tmp, atlas) = fetchFont(inputSource); // CAN THROW

                delete tmp;

                texture = new FontTexture(atlas, inputSource, properties.useMipmap);
                textures[make_pair(inputSource->getURI(), properties.useMipmap)] = unique_ptr<FontTexture>(texture);
                delete atlas;
            }
            else
            {
                texture = it3->second.get();
            }
            
            auto font = new XFont(data, texture, properties);
            fonts[key] = unique_ptr<XFont>(font);
            
            return font;
        }
    }
    
    /*
    bool FontManager::remove(XFont *font)
    {
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (it->second.get() == font)
            {
                cache.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    void FontManager::clear()
    {
        cache.clear();
    }
    */
    
    void FontManager::discardTextures()
    {
        for (auto &it : textures)
        {
            it.second->discard();
        }
    }
    
    void FontManager::reloadTextures()
    {
        for (auto &it : textures)
        {
            FontData *tmp;
            FontAtlas *atlas;
            tie(tmp, atlas) = fetchFont(it.second->inputSource); // CAN THROW
            
            delete tmp;
            
            it.second->upload(atlas);
            delete atlas;
        }
    }
    
    std::pair<FontData*, FontAtlas*> FontManager::fetchFont(InputSourceRef source)
    {
        auto in = source->loadDataSource()->createStream(); // CAN THROW
        
        string version;
        in->readFixedString(&version, 10);
        
        if (version != "XFONT.003")
        {
            throw runtime_error("XFont: WRONG FORMAT");
        }
        
        // ---
        
        int glyphCount;
        in->readLittle(&glyphCount);
        
        auto data = new FontData(glyphCount);
        
        in->readLittle(&data->nativeFontSize);
        in->readLittle(&data->height);
        in->readLittle(&data->ascent);
        in->readLittle(&data->descent);
        in->readLittle(&data->spaceAdvance);
        in->readLittle(&data->strikethroughFactor);
        in->readLittle(&data->underlineOffset);
        in->readLittle(&data->lineThickness);
        
        int atlasWidth;
        int atlasHeight;
        int atlasPadding;
        int unitMargin;
        int unitPadding;
        
        in->readLittle(&atlasWidth);
        in->readLittle(&atlasHeight);
        in->readLittle(&atlasPadding);
        in->readLittle(&unitMargin);
        in->readLittle(&unitPadding);
        
        auto atlas = new FontAtlas(atlasWidth, atlasHeight);
        
        for (int i = 0; i < glyphCount; i++)
        {
            int glyphChar;
            int glyphWidth;
            int glyphHeight;
            int glyphLeftExtent;
            int glyphTopExtent;
            int glyphAtlasX;
            int glyphAtlasY;
            
            in->readLittle(&glyphChar);
            data->glyphs[(wchar_t)glyphChar] = i;
            
            in->readLittle(&data->advance[i]);
            in->readLittle(&glyphWidth);
            in->readLittle(&glyphHeight);
            in->readLittle(&glyphLeftExtent);
            in->readLittle(&glyphTopExtent);
            in->readLittle(&glyphAtlasX);
            in->readLittle(&glyphAtlasY);
            
            auto glyphData = new unsigned char[glyphWidth * glyphHeight];
            in->readData(glyphData, glyphWidth * glyphHeight);
            atlas->addGlyph(glyphData, glyphAtlasX + atlasPadding + unitMargin, glyphAtlasY + atlasPadding + unitMargin, glyphWidth, glyphHeight);
            delete[] glyphData;
            
            data->w[i] = glyphWidth + unitPadding * 2;
            data->h[i] = glyphHeight + unitPadding * 2;
            data->le[i] = glyphLeftExtent - unitPadding;
            data->te[i] = glyphTopExtent + unitPadding;
            
            int x = glyphAtlasX + atlasPadding + unitMargin - unitPadding;
            int y = glyphAtlasY + atlasPadding + unitMargin - unitPadding;
            
            data->u1[i] = x / (float)atlasWidth;
            data->v1[i] = y / (float)atlasHeight;
            data->u2[i] = (x + data->w[i]) / (float)atlasWidth;
            data->v2[i] = (y + data->h[i]) / (float)atlasHeight;
        }
        
        return make_pair(data, atlas);
    }
}
