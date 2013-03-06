#include "chronotext/texture/TextureAtlasAnimation.h"

using namespace std;

TextureAtlasAnimation::TextureAtlasAnimation(TextureAtlas *atlas, const string &path, float fps, bool looping, bool reverse)
:
atlas(atlas),
fps(fps),
looping(looping)
{
    if (reverse)
    {
        vector<Sprite*> tmp = atlas->getAnimationSprites(path);
        
        for (int i = tmp.size() - 1; i >= 0; i--)
        {
            sprites.push_back(tmp[i]);
        }
    }
    else
    {
        sprites = atlas->getAnimationSprites(path);
    }
    
    if (sprites.size() == 0)
    {
        throw std::runtime_error("INVALID ANIMATION");
    }
}

TextureAtlasAnimation::TextureAtlasAnimation(TextureAtlas *atlas, const string &path, float fps, bool looping, int firstFrameIndex, int lastFrameIndex)
:
atlas(atlas),
fps(fps),
looping(looping)
{
    vector<Sprite*> tmp = atlas->getAnimationSprites(path);
    
    if (((firstFrameIndex < 0) || (firstFrameIndex >= tmp.size()) || ((lastFrameIndex < 0) || (lastFrameIndex >= tmp.size()))))
    {
        throw std::runtime_error("OUT-OF-RANGE ANIMATION FRAMES");
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
    
    if (sprites.size() == 0)
    {
        throw std::runtime_error("INVALID ANIMATION");
    }
}

int TextureAtlasAnimation::getFrameCount()
{
    return sprites.size();
}

float TextureAtlasAnimation::getDuration()
{
    return sprites.size() / fps;
}

void TextureAtlasAnimation::beginTexture()
{
    atlas->beginTexture();
}

void TextureAtlasAnimation::endTexture()
{
    atlas->endTexture();
}

void TextureAtlasAnimation::drawFromCenter(int frameIndex)
{
    Sprite *sprite = sprites[looping ? (frameIndex % sprites.size()) : min<int>(frameIndex, sprites.size() - 1)];
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
    Sprite *sprite = sprites[looping ? (frameIndex % sprites.size()) : min<int>(frameIndex, sprites.size() - 1)];
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
