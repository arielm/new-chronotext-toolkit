#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/atlas/TextureAtlas.h"

#include "TextureItem.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    
    std::unique_ptr<TextureAtlas> atlas;
    std::map<std::string, std::unique_ptr<TextureItem>> itemMap;
    std::vector<TextureItem*> sheet;
    int frameCount;

    float scale;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void draw();
    
    void loadSheet(ci::DataSourceRef dataSource);
    void loadAnimation(ci::DataSourceRef dataSource);
    
    void drawFrame(int frameIndex);
};
