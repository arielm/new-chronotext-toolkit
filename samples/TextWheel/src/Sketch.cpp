/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/Context.h"
#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;
using namespace chr::zf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;

void Sketch::setup()
{
    FontManager::LOG_VERBOSE = true;
    
    fontManager.loadConfig(InputSource::getResource("font-config.xml"));
    
    font = fontManager.getFont("babel-serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(48));
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
    
    if (getWindowInfo().aaLevel < 4)
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
    rotation = clock()->getTime() * 0.1f;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);

    // ---
    
    gl::rotate(rotation * R2D);
    
    gl::color(1, 0, 0, 0.125f);
    spiral.drawWire();
    
    font->replaySequence(*sequences[languages[currentLangIndex]]);
}

bool Sketch::keyDown(const KeyEvent &event)
{
    switch (CinderDelegate::getCode(event))
    {
        case KeyEvent::KEY_RIGHT:
            nextVersion();
            return true;
            
        case KeyEvent::KEY_LEFT:
            previousVersion();
            return true;
            
        case KeyEvent::KEY_u:
            fontManager.unload();
            return true;
    }
    
    return false;
}

void Sketch::addVersion(const string &lang)
{
    languages.push_back(lang);

    // ---
    
    string version;
    
    for (auto &line : utils::readLines<string>(InputSource::getResource(lang + ".txt")))
    {
        version += line;
        version += " ";
    }
 
    auto layout = font->createLineLayout(version, LangHelper::toHBLang(lang));
    layouts.emplace(lang, unique_ptr<LineLayout>(layout));
    
    // ---
    
    auto sequence = new FontSequence;
    sequences.emplace(lang, unique_ptr<FontSequence>(sequence));
    
    float offsetX = 3000; // XXX
    float offsetY = font->getOffsetY(*layout, ZFont::ALIGN_MIDDLE);
    
    font->beginSequence(*sequence);
    spiral.drawText(*font, *layout, offsetX, offsetY);
    font->endSequence();
}

void Sketch::nextVersion()
{
    currentLangIndex = utils::math::bound(currentLangIndex + 1, languages.size());
}

void Sketch::previousVersion()
{
    currentLangIndex = utils::math::bound(currentLangIndex - 1, languages.size());
}
