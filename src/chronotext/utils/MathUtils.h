/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/MatrixAffine2.h"
#include "cinder/Rect.h"

#include <vector>
#include <algorithm>

namespace chr
{
    static constexpr float D2R (M_PI / 180.0);
    static constexpr float R2D (180.0 / M_PI);
    
    static constexpr float PI (M_PI);
    static constexpr float TWO_PI (M_PI * 2.0);
    static constexpr float HALF_PI (M_PI * 0.5);

    namespace utils
    {
        namespace math
        {
            inline float boundf(float value, float range)
            {
                float bound = fmodf(value, range);
                return (bound < 0) ? (bound + range) : bound;
            }
            
            inline int bound(int value, int range)
            {
                int bound = value % range;
                return (bound < 0) ? (bound + range) : bound;
            }
            
            /*
             * S-SHAPED CROSS-FADE CURVE: 3 * (t ^ 2) - 2 * (t ^ 3)
             */
            inline float ease(float t)
            {
                return (t * t * (3 - 2 * t));
            }
            
            /*
             * BASED ON QUAKE:
             * http://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root
             */
            inline float fastSqrt(float x)
            {
                int i = *(int*)&x; // store floating-point bits in integer
                i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
                float r = *(float*)&i; // convert new bits into float
                r *= (1.5f - 0.5f * x * r * r); // One round of Newton's method
                return r * x;
            }
            
            inline int nextPowerOfTwo(int x)
            {
                int result = 1;
                
                while (result < x)
                {
                    result <<= 1;
                }
                
                return result;
            }
            
            inline bool isPowerOfTwo(int x)
            {
                return (x > 0) && !(x & (x - 1));
            }
            
            // ---
            
            /*
             * REFERENCE FOR THE FOLLOWING 4: http://stackoverflow.com/a/253874/50335
             */
            
            inline bool approximatelyEqual(float a, float b, float epsilon)
            {
                return fabsf(a - b) <= ((fabsf(a) < fabsf(b) ? fabsf(b) : fabsf(a)) * epsilon);
            }
            
            inline bool essentiallyEqual(float a, float b, float epsilon)
            {
                return fabsf(a - b) <= ((fabsf(a) > fabsf(b) ? fabsf(b) : fabsf(a)) * epsilon);
            }
            
            inline bool definitelyGreaterThan(float a, float b, float epsilon)
            {
                return (a - b) > ((fabsf(a) < fabsf(b) ? fabsf(b) : fabsf(a)) * epsilon);
            }
            
            inline bool definitelyLessThan(float a, float b, float epsilon)
            {
                return (b - a) > ((fabsf(a) < fabsf(b) ? fabsf(b) : fabsf(a)) * epsilon);
            }
            
            // ---
            
            inline bool isNull(const ci::Rectf &rect)
            {
                return (rect.x1 == 0) && (rect.y1 == 0) && (rect.x2 == 0) && (rect.y2 == 0);
            }
            
            inline bool compare(const ci::Rectf &r1, const ci::Rectf &r2)
            {
                return (r1.x1 == r2.x1) && (r1.y1 == r2.y1) && (r1.x2 == r2.x2) && (r1.y2 == r2.y2);
            }
            
            void transformVertices(const std::vector<ci::Vec2f> &source, std::vector<ci::Vec2f> &target, const ci::MatrixAffine2f &matrix);
            
            ci::Rectf getBoundingBox(const std::vector<ci::Vec2f> &polygon);
            float getShortestDistance(const ci::Vec2f &point, const std::vector<ci::Vec2f> &polygon, bool close = false, float threshold = std::numeric_limits<float>::max());
            
            bool isPointInside(const ci::Vec2f &point, const std::vector<ci::Vec2f> &polygon);
            bool isPointInside(const ci::Vec2f &point, const std::vector<std::vector<ci::Vec2f>> &polygons);
        }
    }
}
