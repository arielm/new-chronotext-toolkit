/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FXGDocument.h"

using namespace std;
using namespace ci;

FXGDocument::FXGDocument(DataSourceRef source)
{
    XmlTree doc(source);
    
    if (doc.hasChild("Graphic"))
    {
        auto &graphicElement =  doc.getChild("Graphic");
        auto version = graphicElement.getAttributeValue<float>("version", 0);
        
        if (version == 2)
        {
            viewSize.x = graphicElement.getAttributeValue<float>("viewWidth", 0);
            viewSize.y = graphicElement.getAttributeValue<float>("viewHeight", 0);
            
            parseGroup(graphicElement.getChild("Group"));
        }
    }
}

void FXGDocument::parseGroup(const XmlTree &groupElement)
{
    for (auto &element : groupElement.getChildren())
    {
        if (element->getTag() == "Group")
        {
            parseGroup(*element);
        }
        else if (element->getTag() == "Path")
        {
            auto data = element->getAttributeValue<string>("data");
            auto ox = element->getAttributeValue<float>("x", 0);
            auto oy = element->getAttributeValue<float>("y", 0);
            
            paths.emplace_back(parsePath(data, ox, oy));
        }
    }
}

Path2d FXGDocument::parsePath(const string &data, float ox, float oy)
{
    Path2d out;
    auto it = data.cbegin();
    
    bool inM = false;
    bool inL = false;
    bool inQ = false;
    bool inC = false;
    
    while (it != data.cend())
    {
        char c = *it;
        
        switch (c)
        {
            case 'M':
            case 'L':
            case 'Q':
            case 'C':
            case 'Z':
                it++;
                break;
                
            default:
                c = 0;
                break;
        }
        
        if (c > 0)
        {
            inM = false;
            inL = false;
            inQ = false;
            inC = false;
            
            switch (c)
            {
                case 'M':
                    inM = true;
                    break;
                    
                case 'L':
                    inL = true;
                    break;
                    
                case 'Q':
                    inQ = true;
                    break;
                    
                case 'C':
                    inC = true;
                    break;
                    
                case 'Z':
                    out.close();
                    break;
            }
        }
        
        if (inM)
        {
            float x = nextDouble(data, it);
            float y = nextDouble(data, it);
            
            if (out.empty())
            {
                out.moveTo(x + ox, y + oy);
            }
        }
        else if (inL)
        {
            float x = nextDouble(data, it);
            float y = nextDouble(data, it);
            out.lineTo(x + ox, y + oy);
        }
        else if (inQ)
        {
            float x1 = nextDouble(data, it);
            float y1 = nextDouble(data, it);
            float x2 = nextDouble(data, it);
            float y2 = nextDouble(data, it);
            out.quadTo(x1 + ox, y1 + oy, x2 + ox, y2 + oy);
        }
        else if (inC)
        {
            float x1 = nextDouble(data, it);
            float y1 = nextDouble(data, it);
            float x2 = nextDouble(data, it);
            float y2 = nextDouble(data, it);
            float x3 = nextDouble(data, it);
            float y3 = nextDouble(data, it);
            out.curveTo(x1 + ox, y1 + oy, x2 + ox, y2 + oy, x3 + ox, y3 + oy);
        }
    }
    
    return out;
}

double FXGDocument::nextDouble(const string &input, string::const_iterator &it)
{
    string accum;
    
    while (true)
    {
        if (it == input.end())
        {
            goto exit;
        }
        
        char c = *it++;
        
        switch (c)
        {
            case ' ':
                goto exit;
                break;
                
            case 'M':
            case 'L':
            case 'Q':
            case 'C':
            case 'Z':
                it--;
                goto exit;
                break;
        }
        
        accum += c;
    }
    
exit:
    if (accum.empty())
    {
        throw ;
    }
    else
    {
        return atof(accum.c_str());
    }
}
