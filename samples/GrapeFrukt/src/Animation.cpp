/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Animation.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace chr;

Animation::Animation()
:
fps(1),
frameCount(0)
{}

Animation::Animation(shared_ptr<TextureAtlas> atlas, InputSource::Ref sheetInputSource, InputSource::Ref animationInputSource, float fps)
:
atlas(atlas),
fps(fps),
frameCount(0)
{
    loadSheet(sheetInputSource);
    loadAnimation(animationInputSource);
}

int Animation::getFrameCount() const
{
    return frameCount;
}

float Animation::getDuration() const
{
    return frameCount / fps;
}

void Animation::draw(int frameIndex)
{
    if (atlas)
    {
        atlas->beginTexture();
        
        for (auto &item : itemList)
        {
            auto &frame = item->frames[frameIndex % frameCount];
            
            glPushMatrix();
            glColor4f(1, 1, 1, frame.alpha);
            glTranslatef(frame.x, frame.y, 0);
            glScalef(frame.scaleX, frame.scaleY, 1);
            glRotatef(frame.rotation, 0, 0, 1);
            
            atlas->drawSprite(item->path, item->registrationPointX, item->registrationPointY);
            glPopMatrix();
        }
        
        atlas->endTexture();
    }
}

void Animation::draw(float t)
{
    int frameIndex = t * frameCount;
    draw(frameIndex);
}

void Animation::play(double now)
{
    float t = now / getDuration();
    draw(t);
}

void Animation::loadSheet(InputSource::Ref inputSource)
{
    XmlTree doc(inputSource->loadDataSource());
    
    for (auto textureElement = doc.begin("Textures/TextureSheet/Texture"); textureElement != doc.end(); ++textureElement)
    {
        auto name = textureElement->getAttributeValue<string>("name");
        auto path = textureElement->getAttributeValue<string>("path");
        auto width = textureElement->getAttributeValue<float>("width");
        auto height = textureElement->getAttributeValue<float>("height");
        auto registrationPointX = textureElement->getAttributeValue<float>("registrationPointX");
        auto registrationPointY = textureElement->getAttributeValue<float>("registrationPointY");
        auto zIndex = textureElement->getAttributeValue<int>("zIndex", 0);
        
        auto item = new Item(path, width, height, registrationPointX, registrationPointY, zIndex);
        itemMap[name] = unique_ptr<Item>(item);
        itemList.push_back(item);
    }
    
    std::sort(itemList.begin(), itemList.end(), Item::compareZIndex);
}

void Animation::loadAnimation(InputSource::Ref inputSource)
{
    XmlTree doc(inputSource->loadDataSource());
    
    auto animationElement = doc.getChild("Animations/Animation");
    frameCount = animationElement.getAttributeValue<int>("frameCount");
    
    for (auto partElement = doc.begin("Animations/Animation/Part"); partElement != doc.end(); ++partElement)
    {
        auto name = partElement->getAttributeValue<string>("name");
        auto it = itemMap.find(name);
        
        if (it != itemMap.end())
        {
            auto &item = it->second;
            
            for (auto frameElement = partElement->begin("Frame"); frameElement != partElement->end(); ++frameElement)
            {
                auto x = frameElement->getAttributeValue<float>("x");
                auto y = frameElement->getAttributeValue<float>("y");
                auto scaleX = frameElement->getAttributeValue<float>("scaleX", 1);
                auto scaleY = frameElement->getAttributeValue<float>("scaleY", 1);
                auto rotation = frameElement->getAttributeValue<float>("rotation", 0);
                auto alpha = frameElement->getAttributeValue<float>("alpha", 1);
                
                item->frames.emplace_back(x, y, scaleX, scaleY, rotation, alpha);
            }
        }
        else
        {
            throw runtime_error("ERROR WHILE PARSING GRAPEFRUKT ANIMATION");
        }
    }
}
