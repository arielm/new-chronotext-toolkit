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
    
    // ---
    
    FontTexture::FontTexture(int width, int height, GLuint name)
    :
    width(width),
    height(height),
    name(name)
    {}
    
    FontTexture::~FontTexture()
    {
        glDeleteTextures(1, &name);
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
        auto it1 = cache.find(key);
        
        if (it1 != cache.end())
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
                
                uploadTexture(inputSource, atlas, properties.useMipmap);
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

                texture = uploadTexture(inputSource, atlas, properties.useMipmap);
                delete atlas;
            }
            else
            {
                texture = it3->second.get();
            }
            
            auto font = new XFont(data, texture, properties);
            cache[key] = unique_ptr<XFont>(font);
            
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
    
    void FontManager::unload()
    {
        for (auto &it : cache)
        {
            it.second->unload();
        }
    }
    
    void FontManager::reload()
    {
        for (auto &it : cache)
        {
            it.second->reload();
        }
    }
    */
    
    FontTexture* FontManager::uploadTexture(InputSourceRef source, FontAtlas *atlas, bool useMipmap)
    {
        auto name = uploadAtlas(atlas, useMipmap);
        
        LOGD <<
        "FONT UPLOADED: " <<
        source->getFilePathHint() << " | " <<
        name << " | " <<
        atlas->width << "x" << atlas->height  <<
        (useMipmap ? " (M)" : "") <<
        endl;

        auto texture = new FontTexture(atlas->width, atlas->height, name);
        textures[make_pair(source->getURI(), useMipmap)] = unique_ptr<FontTexture>(texture);
        
        return texture;
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
            addAtlasUnit(atlas, glyphData, glyphAtlasX + atlasPadding + unitMargin, glyphAtlasY + atlasPadding + unitMargin, glyphWidth, glyphHeight);
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
    
    void FontManager::addAtlasUnit(FontAtlas *atlas, unsigned char *glyphData, int x, int y, int width, int height)
    {
        for (int iy = 0; iy < height; iy++)
        {
            for (int ix = 0; ix < width; ix++)
            {
                atlas->data[(iy + y) * atlas->width + ix + x] = glyphData[iy * width + ix];
            }
        }
    }
    
    GLuint FontManager::uploadAtlas(FontAtlas *atlas, bool useMipmap)
    {
        GLuint name;
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlas->width, atlas->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas->data);
        
        if (useMipmap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        return name;
    }
}
