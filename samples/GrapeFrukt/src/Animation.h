/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/atlas/TextureAtlas.h"

class Animation
{
    struct Frame
    {
        float x;
        float y;
        float scaleX;
        float scaleY;
        float rotation;
        float alpha;
        
        Frame(float x, float y, float scaleX, float scaleY, float rotation, float alpha)
        :
        x(x),
        y(y),
        scaleX(scaleX),
        scaleY(scaleY),
        rotation(rotation),
        alpha(alpha)
        {}
    };
    
    struct Item
    {
        std::string path;
        float width;
        float height;
        float registrationPointX;
        float registrationPointY;
        int zIndex;
        
        std::vector<Frame> frames;
        
        Item(const std::string &path, float width, float height, float registrationPointX, float registrationPointY, float zIndex)
        :
        path(path),
        width(width),
        height(height),
        registrationPointX(registrationPointX),
        registrationPointY(registrationPointY),
        zIndex(zIndex)
        {}
        
        static bool compareZIndex(Item *lhs, Item *rhs)
        {
            return (lhs->zIndex < rhs->zIndex);
        }
    };
    
public:
    Animation();
    Animation(std::shared_ptr<chr::TextureAtlas> atlas, chr::InputSource::Ref sheetInputSource, chr::InputSource::Ref animationInputSource, float fps = 24);
    
    int getFrameCount() const;
    float getDuration() const;
    
    void draw(int frameIndex);
    void draw(float t);
    void play(double now);
    
protected:
    std::shared_ptr<chr::TextureAtlas> atlas;
    float fps;
    int frameCount;

    std::map<std::string, std::unique_ptr<Item>> itemMap;
    std::vector<Item*> itemList;

    void loadSheet(chr::InputSource::Ref inputSource);
    void loadAnimation(chr::InputSource::Ref inputSource);
};
