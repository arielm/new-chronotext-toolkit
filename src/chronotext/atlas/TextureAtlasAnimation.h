/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/atlas/TextureAtlas.h"

namespace chronotext
{
    class TextureAtlasAnimation
    {
    public:
        std::vector<SpriteRef> sprites;
        
        TextureAtlasAnimation(TextureAtlas &atlas, const std::string &path, float fps = 24, bool looping = false, bool reverse = false);
        TextureAtlasAnimation(TextureAtlas &atlas, const std::string &path, float fps, bool looping, int firstFrameIndex, int lastFrameIndex);
        
        int getFrameCount() const;
        float getDuration() const;
        
        void beginTexture();
        void endTexture();
        
        void drawFromCenter(int frameIndex);
        void drawFromCenter(float t);
        void playFromCenter(double now);
        
        void draw(int frameIndex, float rx = 0, float ry = 0);
        void draw(float t, float rx = 0, float ry = 0);
        void play(double now, float rx = 0, float ry = 0);
        
    protected:
        TextureAtlas &atlas;
        float fps;
        bool looping;
    };
}

namespace chr = chronotext;
