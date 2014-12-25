/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Path2d.h"
#include "cinder/Xml.h"

namespace chr
{
    class FXGDocument
    {
    public:
        FXGDocument();
        FXGDocument(ci::DataSourceRef source);
        
        ci::Vec2f getViewSize() const;
        const std::vector<ci::Path2d>& getPaths() const;
        
    protected:
        ci::Vec2f viewSize;
        std::vector<ci::Path2d> paths;

        void parseGroup(const ci::XmlTree &groupElement);
        static ci::Path2d parsePath(const std::string &data, float ox, float oy);
        static double nextDouble(const std::string &input, std::string::const_iterator &it);
    };
}
