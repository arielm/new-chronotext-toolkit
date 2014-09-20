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
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float REFERENCE_DENSITY = 160; // THE DENSITY-INDEPENDENT-PIXEL UNIT (DP) IS BASED ON THIS VALUE

const float DOT_RADIUS = 22; // DP
const float DOT_SCALE = 112; // DEPENDS ON IMAGE

const float FONT_SIZE = 24; // DP
const float PADDING = 20; // DP

const int FINGERS_CAPACITY = 10;
const float FINGERS_DISTANCE = 22; // DP

const float FRICTION = 0.01f;
const float DT = 1.0f;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.reload(); // MANDATORY
        fontManager.reloadTextures(); // NOT MANDATORY (GLYPH TEXTURES ARE AUTOMATICALLY RELOADED WHENEVER NECESSARY)
    }
    else
    {
        dot = textureManager.getTexture("dot_112.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Roboto_Regular_64.fnt"), XFont::Properties2d());

        scale = getWindowInfo().density / REFERENCE_DENSITY;
        particle = Particle(getWindowCenter(), scale * DOT_RADIUS);
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
            textureManager.discard();
            break;
    }
}

void Sketch::start(int flags)
{
    acceleration = Vec2f::zero();
	enableAccelerometer(15);
}

void Sketch::stop(int flags)
{
	disableAccelerometer();
}

void Sketch::update()
{
    accumulateForces();
    verlet();
    satifsfyConstraints();
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::color(Color::gray(0.5f));
    drawGrid(getWindowBounds(), scale * FINGERS_DISTANCE * 2, Vec2f(0, clock().getTime() * 60));

    // ---
    
    drawDot(particle.position, particle.radius, ColorA(1, 0, 0, 1));
    
    wstring text = utf8ToWstring(toString(int(clock().getTime())));
    drawText(text, Vec2f(0, getWindowHeight()) + Vec2f(PADDING, -PADDING) * scale, XFont::ALIGN_LEFT, XFont::ALIGN_BOTTOM, scale * FONT_SIZE, ColorA(0, 0, 0, 1));
}

void Sketch::drawDot(const Vec2f &position, float radius, const ColorA &color)
{
    gl::color(color);

    glPushMatrix();
    gl::translate(position);
    gl::scale(2 * radius / DOT_SCALE);
    
    dot->begin();
    dot->drawFromCenter();
    dot->end();
    
    glPopMatrix();
}

void Sketch::drawText(const wstring &text, const Vec2f &position, XFont::Alignment alignX, XFont::Alignment alignY, float fontSize, const ColorA &color)
{
    font->setColor(color);
    font->setSize(fontSize);
    
    TextHelper::drawAlignedText(*font, text, position, alignX, alignY);
}

void Sketch::accelerated(AccelEvent event)
{
    acceleration = Vec2f(+event.getRawData().x, -event.getRawData().y); // FIXME: TAKE IN COUNT DEVICE ORIENTATION ON iOS
}

void Sketch::accumulateForces()
{
    particle.acceleration = particle.mass * acceleration;
}

void Sketch::verlet()
{
    auto tmp = particle.position;
    particle.position = (2 - FRICTION) * particle.position - (1 - FRICTION) * particle.previousPosition + particle.acceleration * DT * DT;
    particle.previousPosition = tmp;
}

void Sketch::satifsfyConstraints()
{
    Rectf bounds(particle.radius, particle.radius, getWindowWidth() - particle.radius, getWindowHeight() - particle.radius);
    auto velocity = particle.position - particle.previousPosition;
    
    if (particle.position.x < bounds.x1)
    {
        particle.position.x = bounds.x1 - velocity.x * 0.5f;
        particle.previousPosition.x = bounds.x1;
    }
    else if (particle.position.x > bounds.x2)
    {
        particle.position.x = bounds.x2 -  velocity.x * 0.5f;
        particle.previousPosition.x = bounds.x2;
    }
    
    if (particle.position.y < bounds.y1)
    {
        particle.position.y = bounds.y1 - velocity.y * 0.5f;
        particle.previousPosition.y = bounds.y1;
    }
    else if (particle.position.y > bounds.y2)
    {
        particle.position.y = bounds.y2 - velocity.y * 0.5f;
        particle.previousPosition.y = bounds.y2;
    }
}
