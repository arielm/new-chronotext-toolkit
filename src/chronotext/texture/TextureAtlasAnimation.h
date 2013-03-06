#pragma once

#include "chronotext/texture/TextureAtlas.h"

#include <vector>

class TextureAtlasAnimation
{
    TextureAtlas *atlas;
    std::vector<Sprite*> sprites;

public:
    float fps;
    bool looping;

    TextureAtlasAnimation(TextureAtlas *atlas, const std::string &path, float fps = 24, bool looping = false, bool reverse = false);
    TextureAtlasAnimation(TextureAtlas *atlas, const std::string &path, float fps, bool looping, int firstFrameIndex, int lastFrameIndex);
    
    int getFrameCount();
    float getDuration();
    
    void beginTexture();
    void endTexture();

    void drawFromCenter(int frameIndex);
    void drawFromCenter(float t);
    void playFromCenter(double now);
    
    void draw(int frameIndex, float rx = 0, float ry = 0);
    void draw(float t, float rx = 0, float ry = 0);
    void play(double now, float rx = 0, float ry = 0);
};
