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
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate),
currentLangIndex(0)
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
        
        font->setSize(TEXT_SIZE);
        font->setColor(0, 0, 0, 0.85f);
        
        spiral.update(0, 0, 67, 500, 17, 1, 40);
        
        addVersion("fr");
        addVersion("he");
        addVersion("da");
        addVersion("ru");
        addVersion("el");
    }
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    if (getWindowAALevel() < 4)
    {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
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
    rotation = now * 0.1f;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);

    // ---
    
    glRotatef(rotation * R2D, 0, 0, 1);
    
    gl::color(1, 0, 0, 0.125f);
    spiral.drawWire();
    
    font->replaySequence(sequences[languages[currentLangIndex]]);
}

void Sketch::addVersion(const string &lang)
{
    languages.push_back(lang);
    
    auto version = readLines<string>(InputSource::getResource(lang + ".txt"));
    string buffer;
    
    for (auto &line : version)
    {
        buffer += line;
        buffer += " ";
    }
 
    versions[lang] = unique_ptr<LineLayout>(font->createLineLayout(buffer, lang));
    
    // ---
    
    sequences[lang] = FontSequence();
    
    float offsetX = 3000; // XXX
    float offsetY = font->getOffsetY(*versions[lang], ZFont::ALIGN_MIDDLE);
    
    font->beginSequence(sequences[lang]);
    spiral.drawText(*font, *versions[lang], offsetX, offsetY);
    font->endSequence();
}

void Sketch::nextVersion()
{
    currentLangIndex = bound(currentLangIndex + 1, languages.size());
}

void Sketch::previousVersion()
{
    currentLangIndex = bound(currentLangIndex - 1, languages.size());
}
