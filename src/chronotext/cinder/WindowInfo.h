/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Area.h"

namespace chr
{
    struct WindowInfo
    {
        ci::Vec2i size;
        int aaLevel;

        static WindowInfo create(int width, int height, int aaLevel = 0)
        {
            return WindowInfo(width, height, aaLevel);
        }
        
        WindowInfo(int width, int height, int aaLevel = 0)
        :
        size(width, height),
        aaLevel(aaLevel)
        {}

        WindowInfo(const ci::Vec2i &size = ci::Vec2i::zero(), int aaLevel = 0)
        :
        size(size),
        aaLevel(aaLevel)
        {}
        
        void rotate()
        {
            size = size.yx();
        }
        
        ci::Vec2i getSize() const
        {
            return size;
        }
        
        float getWidth() const
        {
            return size.x;
        }
        
        float getHeight() const
        {
            return size.y;
        }
        
        ci::Area getBounds() const
        {
            return ci::Area(0, 0, size.x, size.y);
        }
        
        ci::Vec2f getCenter() const
        {
            return size * 0.5f;
        }
        
        float getAspectRatio() const
        {
            if (size.y > 0)
            {
                return size.x / float(size.y);
            }
            else
            {
                return 0;
            }
        }
        
        friend std::ostream& operator<<(std::ostream &lhs, const WindowInfo &rhs)
        {
            lhs
            << "{"
            << "size: " << rhs.size
            << ", anti-alasing-level: " << rhs.aaLevel
            << "}";
            
            return lhs;
        }
    };
}
