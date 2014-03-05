/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/zf/FontManager.h"

#include "TextSpiral.h"

class Sketch : public chr::CinderSketch
{
    chr::zf::FontManager fontManager;

    float scale;
    float rotation;

    std::shared_ptr<chr::ZFont> font;
    chr::zf::FontSequence sequence;
    TextSpiral spiral;
    
    std::map<std::string, std::unique_ptr<chr::zf::LineLayout>> versions;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void event(int id);
    void resize();
    void update();
    void draw();
    
    void addVersion(const std::string &lang);
    chr::zf::LineLayout& getVersion(const std::string &lang);
};
