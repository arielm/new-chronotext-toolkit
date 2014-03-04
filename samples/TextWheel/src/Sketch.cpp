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
currentLineIndex(0)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        /*
         *  NECESSARY AFTER OPEN-GL CONTEXT-LOSS (OCCURS ON ANDROID WHEN APP GOES TO BACKGROUND)
         */
        fontManager.discardTextures();
        fontManager.reloadTextures(); // NOT MANDATORY (GLYPHS TEXTURE ARE AUTOMATICALLY RELOADED WHENEVER NECESSARY)
    }
    else
    {
        font = fontManager.getCachedFont(InputSource::getResource("babel_osx.xml"), ZFont::Properties2d(48));
        
        addVersion("he");
        addVersion("fr");
        addVersion("da");
        addVersion("ru");
        addVersion("el");
    }
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(getWindowHeight() / REFERENCE_H);

    // ---
    
    auto &layout = getNextLine(); // lines["ru"][0];
    auto offset = font->getOffset(*layout, ZFont::ALIGN_MIDDLE, ZFont::ALIGN_MIDDLE);
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    
    font->beginSequence();
    
    for (auto &cluster : layout->clusters)
    {
        font->addCluster(cluster, offset);
        offset.x += font->getAdvance(cluster);
    }
    
    font->endSequence();
}

void Sketch::addVersion(const string &lang)
{
    languages.push_back(lang);
    
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
