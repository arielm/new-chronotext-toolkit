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
    
    for (XmlTree::Iter textureElement = doc.begin("Textures/TextureSheet/Texture"); textureElement != doc.end(); ++textureElement)
    {
        string name = textureElement->getAttributeValue<string>("name");
        string path = textureElement->getAttributeValue<string>("path");
        
        float width = textureElement->getAttributeValue<float>("width");
        float height = textureElement->getAttributeValue<float>("height");
        
        float registrationPointX = textureElement->getAttributeValue<float>("registrationPointX");
        float registrationPointY = textureElement->getAttributeValue<float>("registrationPointY");
        
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
    
    const XmlTree animationElement = doc.getChild("Animations/Animation");
    frameCount = animationElement.getAttributeValue<int>("frameCount");
    
    for (XmlTree::Iter partElement = doc.begin("Animations/Animation/Part"); partElement != doc.end(); ++partElement)
    {
        string name = partElement->getAttributeValue<string>("name");
        auto it = itemMap.find(name);
        
        if (it != itemMap.end())
        {
            auto &item = it->second;
            
            for (XmlTree::Iter frameElement = partElement->begin("Frame"); frameElement != partElement->end(); ++frameElement)
            {
                float x = frameElement->getAttributeValue<float>("x");
                float y = frameElement->getAttributeValue<float>("y");
                float scaleX = frameElement->getAttributeValue<float>("scaleX", 1);
                float scaleY = frameElement->getAttributeValue<float>("scaleY", 1);
                float rotation = frameElement->getAttributeValue<float>("rotation", 0);
                float alpha = frameElement->getAttributeValue<float>("alpha", 1);
                
                item->frames.push_back(Frame(x, y, scaleX, scaleY, rotation, alpha));
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
