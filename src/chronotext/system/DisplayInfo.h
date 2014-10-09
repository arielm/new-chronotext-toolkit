/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Vector.h"

namespace chronotext
{
    class DisplayInfo
    {
    public:
        constexpr static float REFERENCE_DENSITY = 160; // THE DENSITY-INDEPENDENT-PIXEL UNIT (DP) IS BASED ON THIS VALUE
        
        static DisplayInfo create(int width, int height, float contentScale = 1)
        {
            return DisplayInfo(ci::Vec2i(width, height), contentScale, 0, 0);
        }
        
        static DisplayInfo createWithDiagonal(int width, int height, float diagonal, float contentScale)
        {
            return DisplayInfo(ci::Vec2i(width, height), contentScale, diagonal, 0);
        }

        static DisplayInfo createWithDiagonal(int width, int height, float diagonal)
        {
            return createWithDiagonal(width, height, diagonal, 1);
        }

        static DisplayInfo createWithDensity(int width, int height, float density, float contentScale)
        {
            return DisplayInfo(ci::Vec2i(width, height), contentScale, 0, density);
        }
        
        static DisplayInfo createWithDensity(int width, int height, float density)
        {
            return createWithDensity(width, height, density, 1);
        }
        
        DisplayInfo()
        :
        contentScale(0),
        diagonal(0),
        density(0),
        fullSize(ci::Vec2i::zero()),
        aspectRatio(0),
        valid(false)
        {}
        
        void rotate()
        {
            if (valid)
            {
                baseSize = baseSize.yx();
                fullSize = fullSize.yx();
                
                aspectRatio = fullSize.x / float(fullSize.y);
            }
        }
        
        float getContentScale() const
        {
            return contentScale;
        }

        float getDiagonal() const
        {
            return diagonal; // INCHES
        }
        
        float getDensity() const
        {
            return density; // DPI
        }
        
        ci::Vec2i getSize() const
        {
            return fullSize; // PIXELS
        }

        float getAspectRatio() const
        {
            return aspectRatio;
        }

        bool isLandscape() const
        {
            return (aspectRatio > 1);
        }

        friend std::ostream& operator<<(std::ostream &lhs, const DisplayInfo &rhs)
        {
            lhs
            << "{"
            << "size: " << rhs.fullSize
            << ", contentScale: " << rhs.contentScale
            << ", diagonal: " << rhs.diagonal
            << ", density: " << rhs.density
            << "}";
            
            return lhs;
        }
        
    protected:
        ci::Vec2i baseSize;
        float contentScale;
        float diagonal;
        float density;
        
        ci::Vec2i fullSize;
        float aspectRatio;
        
        bool valid;
        
        DisplayInfo(const ci::Vec2i &baseSize, float contentScale, float diagonal, float density)
        :
        baseSize(baseSize),
        contentScale(contentScale),
        diagonal(diagonal),
        density(density)
        {
           valid = update();
        }
        
        bool update()
        {
            if ((baseSize.x * baseSize.y == 0) || (contentScale < 1) || (diagonal < 0) || (density < 0))
            {
                contentScale = 0;
                diagonal = 0;
                density = 0;
                
                fullSize = ci::Vec2i::zero();
                aspectRatio = 0;
                
                return false;
            }
            
            fullSize = baseSize * contentScale;
            aspectRatio = fullSize.x / float(fullSize.y);
            
            if ((diagonal == 0) && (density == 0))
            {
                density = REFERENCE_DENSITY;
            }
            
            if (diagonal > 0)
            {
                density = fullSize.length() / diagonal;
            }
            else
            {
                diagonal = fullSize.length() / density;
            }
            
            return true;
        }
    };
}

namespace chr = chronotext;
