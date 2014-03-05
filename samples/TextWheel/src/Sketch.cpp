/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/Utils.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate),
currentLangIndex(0),
currentLineIndex(0),
position(0)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        fontManager.reloadTextures(); // NOT MANDATORY (GLYPHS TEXTURE ARE AUTOMATICALLY RELOADED WHENEVER NECESSARY)
    }
    else
    {
        font = fontManager.getCachedFont(InputSource::getResource("babel_osx.xml"), ZFont::Properties2d(48));
        
        // ---
        
        addVersion("he");
        addVersion("fr");
        addVersion("da");
        addVersion("ru");
        addVersion("el");
        
        // ---
        
        spiral.update(0, 0, 67, 500, 17, 0.5f, 50);
    }
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::event(int id)
{
    switch (id)
    {
        case EVENT_CONTEXT_LOST:
            fontManager.discardTextures();
            break;
    }
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::update()
{
    double now = getElapsedSeconds();
    position = now * 40;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);

    // ---
    
    gl::color(1, 0, 0, 0.5f);
    spiral.drawWire();
    
    // ---
    
    auto &layout = lines["fr"][0];
    float offsetY = font->getOffsetY(*layout, ZFont::ALIGN_MIDDLE);
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    
    font->beginSequence();
    spiral.drawText(*font, *layout, position, offsetY);
    font->endSequence();
}

void Sketch::addVersion(const string &lang)
{
    languages.push_back(lang); // WE CAN'T USE A std::set BECAUSE ORDER OF INSERTION MATTERS
    
    auto version = readLines<string>(InputSource::getResource(lang + ".txt"));
    
    for (auto &textLine : version)
    {
        lines[lang].emplace_back(unique_ptr<LineLayout>(font->createLineLayout(textLine, lang)));
    }
}

unique_ptr<LineLayout>& Sketch::getNextLine()
{
    auto &version = lines[languages[currentLangIndex]];
    auto &line = version[currentLineIndex];
    
    currentLineIndex = (currentLineIndex + 1) % version.size();
    
    if (currentLineIndex == 0)
    {
        currentLangIndex = (currentLangIndex + 1) % languages.size();
    }
    
    return line;
}
