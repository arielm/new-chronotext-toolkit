/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TextSpiral.h"

#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

void TextSpiral::update(float ox, float oy, float r1, float r2, float turns, float DD1, float DD2)
{
    this->ox = ox;
    this->oy = oy;
    this->r1 = r1;
    this->r2 = r2;
    this->turns = turns;
    
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
        
        vertices.emplace_back(ox - math<float>::cos(d) * r, oy + math<float>::sin(d) * r);
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

void TextSpiral::drawText(ZFont &font, const LineLayout &layout, float offsetX, float offsetY)
{
    bool reverse = (layout.overallDirection == HB_DIRECTION_RTL);

    float l = TWO_PI * turns;
    float dr = (r2 - r1) / l;
    float D = offsetX;
    
    auto matrix = font.getMatrix();
    
    for (auto &cluster : DirectionalRange(layout.clusters, reverse))
    {
        float half = 0.5f * font.getAdvance(cluster);
        D += half;
        
        if (!cluster.isSpace)
        {
            float r = math<float>::sqrt(r1 * r1 + 2 * dr * D);
            float d = (r - r1) / dr;
            
            matrix->setTranslation(ox - math<float>::cos(d) * r, oy + math<float>::sin(d) * r);
            matrix->rotateZ((reverse ? -1 : +1) * HALF_PI - d);
            
            font.addTransformedCluster(cluster, -half, offsetY);
        }
        
        D += half;
    }
}
