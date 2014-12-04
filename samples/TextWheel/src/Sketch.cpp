/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/Utils.h"
#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;

Sketch::Sketch()
:
CinderSketch()
{}

void Sketch::setup()
{
    fontManager.loadConfig(InputSource::getResource("font-config.xml"));
    
    font = fontManager.getCachedFont("babel-serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(48));
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    
    spiral.update(0, 0, 67, 500, 17, 1, 40);
    
    addVersion("he"); // HEBREW
    addVersion("en"); // ENGLISH
    addVersion("zh-tw"); // CHINESE (TRADITIONAL)
    addVersion("fr"); // FRENCH
    addVersion("ru"); // RUSSIAN
    addVersion("ko"); // KOREAN
    addVersion("de"); // GERMAN
    addVersion("hi"); // HINDI
    addVersion("es"); // SPANISH
    addVersion("ja"); // JAPANESE
    addVersion("ar"); // ARABIC
    addVersion("el"); // GREEK
    
    currentLangIndex = Rand::randInt(0, languages.size() - 1);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    if (windowInfo().aaLevel < 4)
    {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::update()
{
    rotation = getElapsedSeconds() * 0.1f;
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

    // ---
    
    string version;
    
    for (auto &line : readLines<string>(InputSource::getResource(lang + ".txt")))
    {
        version += line;
        version += " ";
    }
 
    auto layout = font->createLineLayout(version, lang);
    layouts[lang] = unique_ptr<LineLayout>(layout);
    
    // ---
    
    sequences.emplace(lang); // CONSTRUCTS A NEW FontSequence INTO THE MAP
    
    float offsetX = 3000; // XXX
    float offsetY = font->getOffsetY(*layout, ZFont::ALIGN_MIDDLE);
    
    font->beginSequence(sequences[lang]);
    spiral.drawText(*font, *layout, offsetX, offsetY);
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
