/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TextSpiral.h"

#include "chronotext/utils/MathUtils.h"

#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace chr;

void TextSpiral::update(float ox, float oy, float r1, float r2, float turns, float DD1, float DD2)
{
    vertices.clear();

    float l = TWO_PI * turns;
    float L = PI * turns * (r1 + r2);
    float dr = (r2 - r1) / l;
    float DD = (DD2 - DD1) / l;
    float D = 0;
    
    do
    {
        float r = math<float>::sqrt(r1 * r1 + 2 * dr * D);
        float d = (r - r1) / dr;
        D += DD1 + d * DD;
        
        vertices.emplace_back(ox - math<float>::sin(d) * r, oy + math<float>::cos(d) * r);
    }
    while (D < L);
}

void TextSpiral::drawWire()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
}
