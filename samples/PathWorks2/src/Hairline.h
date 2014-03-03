#pragma once

#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/StrokeHelper.h"

class Hairline
{
public:
    typedef enum
    {
        TYPE_NORMAL,
        TYPE_DASHED
    }
    Type;
    
    Hairline();
    Hairline(chr::TextureManager &textureManager, Type type, bool highDensity);
    
    void stroke(const chr::FollowablePath &path, float scale = 1, float position = 0);
    
    void beginTexture();
    void endTexture();
    void draw();
    
protected:
    Type type;
    
    chr::TextureRef texture;
    chr::TexturedTriangleStrip strip;
};
