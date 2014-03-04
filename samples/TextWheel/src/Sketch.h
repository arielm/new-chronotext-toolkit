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

class Sketch : public chr::CinderSketch
{
    chr::zf::FontManager fontManager;

    float scale;

    std::shared_ptr<chr::ZFont> font;
    std::vector<std::string> languages;
    std::map<std::string, std::vector<std::unique_ptr<chr::zf::LineLayout>>> lines;
    
    int currentLangIndex;
    int currentLineIndex;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void draw();
    
    void addVersion(const std::string &lang);
    std::unique_ptr<chr::zf::LineLayout>& getNextLine();
};
