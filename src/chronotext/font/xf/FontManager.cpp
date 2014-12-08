/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/xf/FontManager.h"
#include "chronotext/utils/Utils.h"

#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

namespace chr
{
    namespace xf
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
        
        void FontAtlas::addGlyph(unsigned char *glyphData, int glyphX, int glyphY, int glyphWidth, int glyphHeight)
        {
            for (int y = 0; y < glyphHeight; y++)
            {
                for (int x = 0; x < glyphWidth; x++)
                {
                    data[(y + glyphY) * width + x + glyphX] = glyphData[y * glyphWidth + x];
                }
            }
        }
        
        // ---
        
        FontTexture::FontTexture(FontAtlas *atlas, InputSource::Ref inputSource)
        :
        width(atlas->width),
        height(atlas->height),
        id(0),
        inputSource(inputSource)
        {
            upload(atlas);
        }
        
        FontTexture::~FontTexture()
        {
            discard();
        }
        
        void FontTexture::upload(FontAtlas *atlas)
        {
            assert(width == atlas->width);
            assert(height == atlas->height);
            
            if (!id)
            {
                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas->data);
                
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
                glBindTexture(GL_TEXTURE_2D, 0);
                
                // ---
                
                LOGD <<
                "FONT UPLOADED: " <<
                inputSource->getFilePathHint() << " | " <<
                id << " | " <<
                width << "x" << height  <<
                endl;
            }
        }
        
        void FontTexture::discard()
        {
            if (id)
            {
                LOGD <<
                "FONT DISCARDED: " <<
                id <<
                endl;
                
                // ---
                
                glDeleteTextures(1, &id);
                id = 0;
            }
        }
        
        void FontTexture::reload()
        {
            if (!id)
            {
                FontData *data;
                FontAtlas *atlas;
                tie(data, atlas) = FontManager::fetchFontDataAndAtlas(inputSource); // CAN THROW
                
                delete data; // WE'RE ONLY INTERESTED IN THE FontAtlas
                
                upload(atlas);
                delete atlas;
            }
        }
        
        void FontTexture::bind()
        {
            reload();
            glBindTexture(GL_TEXTURE_2D, id);
        }
        
        size_t FontTexture::getMemoryUsage() const
        {
            if (id)
            {
                return size_t(width * height * 1.333f);
            }
            else
            {
                return 0;
            }
        }
        
        // ---
        
        std::shared_ptr<Font> FontManager::getCachedFont(InputSource::Ref inputSource, const Font::Properties &properties)
        {
            auto uri = inputSource->getURI();
            
            auto key = make_pair(uri, properties);
            auto it1 = fonts.find(key);
            
            if (it1 != fonts.end())
            {
                return it1->second;
            }
            else
            {
                FontData *data;
                FontTexture *texture;
                auto it2 = fontDataAndTextures.find(uri);
                
                if (it2 == fontDataAndTextures.end())
                {
                    FontAtlas *atlas;
                    tie(data, atlas) = fetchFontDataAndAtlas(inputSource); // CAN THROW
                    
                    texture = new FontTexture(atlas, inputSource);
                    delete atlas;
                    
                    fontDataAndTextures[uri] = make_pair(unique_ptr<FontData>(data), unique_ptr<FontTexture>(texture));
                }
                else
                {
                    data = it2->second.first.get();
                    texture = it2->second.second.get();
                }
                
                auto font = shared_ptr<Font>(new Font(*this, data, texture, properties)); // make_shared WON'T WORK WITH A PROTECTED CONSTRUCTOR
                fonts[key] = font;
                
                return font;
            }
        }
        
        void FontManager::unload(shared_ptr<Font> font)
        {
            for (auto it = fonts.begin(); it != fonts.end(); ++it)
            {
                if (it->second == font)
                {
                    fonts.erase(it);
                    break;
                }
            }
            
            discardUnusedTextures();
        }
        
        void FontManager::unload(InputSource::Ref inputSource)
        {
            for (auto it = fonts.begin(); it != fonts.end();)
            {
                if (it->first.first == inputSource->getURI())
                {
                    it = fonts.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            
            discardUnusedTextures();
        }
        
        void FontManager::unload()
        {
            fonts.clear();
            discardTextures();
        }
        
        void FontManager::discardTextures()
        {
            for (auto &it : fontDataAndTextures)
            {
                it.second.second->discard();
            }
        }
        
        void FontManager::reloadTextures()
        {
            set<FontTexture*> texturesInUse;
            
            for (auto &it1 : fonts)
            {
                auto &uri = it1.first.first;
                auto it2 = fontDataAndTextures.find(uri);
                
                if (it2 != fontDataAndTextures.end())
                {
                    texturesInUse.insert(it2->second.second.get());
                }
            }
            
            for (auto &texture : texturesInUse)
            {
                texture->reload();
            }
        }
        
        size_t FontManager::getTextureMemoryUsage() const
        {
            size_t total = 0;
            
            for (auto &it : fontDataAndTextures)
            {
                total += it.second.second->getMemoryUsage();
            }
            
            return total;
        }
        
        void FontManager::discardUnusedTextures()
        {
            set<FontTexture*> texturesInUse;
            
            for (auto &it1 : fonts)
            {
                auto &uri = it1.first.first;
                auto it2 = fontDataAndTextures.find(uri);
                
                if (it2 != fontDataAndTextures.end())
                {
                    texturesInUse.insert(it2->second.second.get());
                }
            }
            
            for (auto &it : fontDataAndTextures)
            {
                if (!texturesInUse.count(it.second.second.get()))
                {
                    it.second.second->discard();
                }
            }
        }
        
        std::pair<FontData*, FontAtlas*> FontManager::fetchFontDataAndAtlas(InputSource::Ref source)
        {
            auto in = source->loadDataSource()->createStream(); // CAN THROW
            
            string version;
            in->readFixedString(&version, 10);
            
            if (version != "XFONT.004")
            {
                throw EXCEPTION(xf::FontManager, "WRONG FORMAT");
            }
            
            // ---
            
            int glyphCount;
            in->readLittle(&glyphCount);
            
            auto data = new FontData(glyphCount);
            
            in->readLittle(&data->baseSize);
            in->readLittle(&data->height);
            in->readLittle(&data->ascent);
            in->readLittle(&data->descent);
            in->readLittle(&data->lineThickness);
            in->readLittle(&data->underlineOffset);
            in->readLittle(&data->strikethroughOffset);
            in->readLittle(&data->spaceAdvance);
            
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
        
        const vector<uint16_t>& FontManager::getIndices(int capacity)
        {
            if (capacity * 6 > indices.size())
            {
                /*
                 * FILLING THE INDICES WITH A QUAD PATTERN:
                 * - FROM UPPER-LEFT CORNER
                 * - COUNTER-CLOCKWISE
                 */
                
                indices.clear();
                indices.reserve(capacity * 6);
                
                int offset = 0;
                
                for (int i = 0; i < capacity; i++)
                {
                    indices.push_back(offset);
                    indices.push_back(offset + 1);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 3);
                    indices.push_back(offset);
                    offset += 4;
                }
            }
            
            return indices;
        }
    }
}
