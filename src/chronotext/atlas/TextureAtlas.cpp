/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/atlas/TextureAtlas.h"

#include "cinder/Xml.h"

using namespace ci;
using namespace std;

namespace chr
{
    TextureAtlas::TextureAtlas(TextureManager &textureManager, const string &resourceName, bool useMipmap)
    :
    textureManager(textureManager)
    {
        init(InputSource::getResource(resourceName), useMipmap);
    }
    
    TextureAtlas::TextureAtlas(TextureManager &textureManager, InputSourceRef inputSource, bool useMipmap)
    :
    textureManager(textureManager)
    {
        init(inputSource, useMipmap);
    }
    
    void TextureAtlas::init(InputSourceRef inputSource, bool useMipmap)
    {
        XmlTree doc(inputSource->loadDataSource());
        
        auto texturePath = doc.getChild("TextureAtlas").getAttributeValue<string>("imagePath");
        auto textureSource = inputSource->getSubSource(texturePath);
        
        texture = textureManager.getTexture(textureSource, useMipmap);
        float width = texture->getWidth();
        float height = texture->getHeight();
        
        // ---
        
        for (auto spriteElement = doc.begin("TextureAtlas/sprite"); spriteElement != doc.end(); ++spriteElement)
        {
            auto spritePath = spriteElement->getAttributeValue<string>("n");
            
            auto x = spriteElement->getAttributeValue<float>("x");
            auto y = spriteElement->getAttributeValue<float>("y");
            auto w = spriteElement->getAttributeValue<float>("w");
            auto h = spriteElement->getAttributeValue<float>("h");
            auto rotated = spriteElement->hasAttribute("r");
            
            float ox, oy;
            float ow, oh;
            
            if (rotated)
            {
                oy = spriteElement->getAttributeValue<float>("oX", 0);
                ox = spriteElement->getAttributeValue<float>("oY", 0);
                ow = spriteElement->getAttributeValue<float>("oW", h);
                oh = spriteElement->getAttributeValue<float>("oH", w);
            }
            else
            {
                ox = spriteElement->getAttributeValue<float>("oX", 0);
                oy = spriteElement->getAttributeValue<float>("oY", 0);
                ow = spriteElement->getAttributeValue<float>("oW", w);
                oh = spriteElement->getAttributeValue<float>("oH", h);
            }
            
            float u1 = x / width;
            float v1 = y / height;
            
            float u2 = (x + w) / width;
            float v2 = (y + h) / height;
            
            sprites[spritePath] = make_shared<Sprite>(texture, w, h, ox, oy, ow, oh, rotated, u1, v1, u2, v2);
        }
    }
    
    SpriteRef TextureAtlas::getSprite(const string &path) const
    {
        auto it = sprites.find(path);
        
        if (it == sprites.end())
        {
            return SpriteRef();
        }
        else
        {
            return it->second;
        }
    }
    
    vector<SpriteRef> TextureAtlas::getAnimationSprites(const string &path) const
    {
        vector<SpriteRef> animationSprites;
        string pattern = path + "%d";
        
        for (auto &it : sprites)
        {
            int i = -1;
            sscanf(it.first.c_str(), pattern.c_str(), &i);
            
            if (i != -1)
            {
                animationSprites.push_back(it.second);
            }
        }
        
        return animationSprites;
    }
    
    void TextureAtlas::beginTexture()
    {
        texture->begin();
    }
    
    void TextureAtlas::endTexture()
    {
        texture->end();
    }
    
    void TextureAtlas::drawSprite(const string &path, float rx, float ry)
    {
        auto it = sprites.find(path);
        
        if (it != sprites.end())
        {
            it->second->draw(rx, ry);
        }
    }
    
    void TextureAtlas::drawSpriteFromCenter(const std::string &path)
    {
        auto it = sprites.find(path);
        
        if (it != sprites.end())
        {
            it->second->drawFromCenter();
        }
    }
}
