/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/atlas/TextureAtlasAnimation.h"

using namespace std;

namespace chr
{
    TextureAtlasAnimation::TextureAtlasAnimation(TextureAtlas &atlas, const string &path, float fps, bool looping, bool reverse)
    :
    atlas(atlas),
    fps(fps),
    looping(looping)
    {
        if (reverse)
        {
            auto tmp = atlas.getAnimationSprites(path);
            
            for (int i = tmp.size() - 1; i >= 0; i--)
            {
                sprites.push_back(tmp[i]);
            }
        }
        else
        {
            sprites = atlas.getAnimationSprites(path);
        }
        
        if (sprites.empty())
        {
            throw EXCEPTION(TextureAtlasAnimation, "INVALID ANIMATION");
        }
    }
    
    TextureAtlasAnimation::TextureAtlasAnimation(TextureAtlas &atlas, const string &path, float fps, bool looping, int firstFrameIndex, int lastFrameIndex)
    :
    atlas(atlas),
    fps(fps),
    looping(looping)
    {
        auto tmp = atlas.getAnimationSprites(path);
        
        if (((firstFrameIndex < 0) || (firstFrameIndex >= tmp.size()) || ((lastFrameIndex < 0) || (lastFrameIndex >= tmp.size()))))
        {
            throw EXCEPTION(TextureAtlasAnimation, "OUT-OF-RANGE ANIMATION FRAMES");
        }
        
        if (firstFrameIndex < lastFrameIndex)
        {
            for (int i = firstFrameIndex; i <= lastFrameIndex; i++)
            {
                sprites.push_back(tmp[i]);
            }
        }
        else
        {
            for (int i = firstFrameIndex; i >= lastFrameIndex; i--)
            {
                sprites.push_back(tmp[i]);
            }
        }
        
        if (sprites.empty())
        {
            throw EXCEPTION(TextureAtlasAnimation, "INVALID ANIMATION");
        }
    }
    
    int TextureAtlasAnimation::getFrameCount() const
    {
        return sprites.size();
    }
    
    float TextureAtlasAnimation::getDuration() const
    {
        return sprites.size() / fps;
    }
    
    void TextureAtlasAnimation::beginTexture()
    {
        atlas.beginTexture();
    }
    
    void TextureAtlasAnimation::endTexture()
    {
        atlas.endTexture();
    }
    
    void TextureAtlasAnimation::drawFromCenter(int frameIndex)
    {
        auto sprite = sprites[looping ? (frameIndex % sprites.size()) : min<int>(frameIndex, sprites.size() - 1)];
        sprite->drawFromCenter();
    }
    
    void TextureAtlasAnimation::drawFromCenter(float t)
    {
        int frameIndex = t * sprites.size();
        drawFromCenter(frameIndex);
    }
    
    void TextureAtlasAnimation::playFromCenter(double now)
    {
        float t = now / getDuration();
        drawFromCenter(t);
    }
    
    void TextureAtlasAnimation::draw(int frameIndex, float rx, float ry)
    {
        auto sprite = sprites[looping ? (frameIndex % sprites.size()) : min<int>(frameIndex, sprites.size() - 1)];
        sprite->draw(rx, ry);
    }
    
    void TextureAtlasAnimation::draw(float t, float rx, float ry)
    {
        int frameIndex = t * sprites.size();
        draw(frameIndex, rx, ry);
    }
    
    void TextureAtlasAnimation::play(double now, float rx, float ry)
    {
        float t = now / getDuration();
        draw(t, rx, ry);
    }
}
