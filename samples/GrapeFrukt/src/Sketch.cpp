#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace chr;

const float GRID_SIZE = 32;
const float SCALE = 600;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate),
frameCount(0)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.unload();
        textureManager.reload();
    }
    else
    {
        atlas = unique_ptr<TextureAtlas>(new TextureAtlas("MonocleMan.xml"));
        
        loadSheet(InputSource::loadResource("sheets.xml"));
        loadAnimation(InputSource::loadResource("animations.xml"));
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / SCALE;
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    glScalef(scale, scale, 1);
    
    gl::translate(getWindowCenter() / scale);
    drawFrame(getElapsedFrames() % frameCount);
}

void Sketch::loadSheet(DataSourceRef dataSource)
{
    XmlTree doc(dataSource);
    
    for (auto textureElement = doc.begin("Textures/TextureSheet/Texture"); textureElement != doc.end(); ++textureElement)
    {
        auto name = textureElement->getAttributeValue<string>("name");
        auto path = textureElement->getAttributeValue<string>("path");
        auto width = textureElement->getAttributeValue<float>("width");
        auto height = textureElement->getAttributeValue<float>("height");
        auto registrationPointX = textureElement->getAttributeValue<float>("registrationPointX");
        auto registrationPointY = textureElement->getAttributeValue<float>("registrationPointY");
        int zIndex = textureElement->getAttributeValue<int>("zIndex", 0);
        
        auto item = new TextureItem(path, width, height, registrationPointX, registrationPointY, zIndex);
        itemMap[name] = unique_ptr<TextureItem>(item);
        sheet.push_back(item);
    }
    
    std::sort(sheet.begin(), sheet.end(), TextureItem::compareZIndex);
}

void Sketch::loadAnimation(DataSourceRef dataSource)
{
    XmlTree doc(dataSource);
    
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

void Sketch::drawFrame(int frameIndex)
{
    atlas->beginTexture();
    
    for (auto &item : sheet)
    {
        auto &frame = item->frames[frameIndex];
        
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
