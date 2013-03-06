#pragma once

#include "cinder/Easing.h"
#include "cinder/Function.h"

namespace chronotext
{
    typedef std::function< float(float) > Curve;
    
    // ---
    
    struct CurveOne
    {
        float operator() (float t) const
        {
            return 1;
        }
    };
    
    struct CurveLinear
    {
        float from;
        float to;
        
        CurveLinear(float from, float to) : from(from), to(to) {}
        
        float operator() (float t) const
        {
            return from + (to - from) * t;
        }
    };
    
    struct CurveEaseInQuad
    {
        float from;
        float to;
        
        CurveEaseInQuad(float from, float to) : from(from), to(to) {}
        
        float operator() (float t) const
        {
            return from + (to - from) * ci::easeInQuad(t);
        }
    };
    
    struct CurveEaseOutQuad
    {
        float from;
        float to;
        
        CurveEaseOutQuad(float from, float to) : from(from), to(to) {}
        
        float operator() (float t) const
        {
            return from + (to - from) * ci::easeOutQuad(t);
        }
    };
    
    // ---
    
    inline float sineBell(float t)
    {
        return ci::math<float>::sin(t * M_PI);
    }
    
    struct CurveSineBell
    {
        float from;
        float to;
        
        CurveSineBell(float from, float to) : from(from), to(to) {}
        
        float operator() (float t) const
        {
            return from + (to - from) * sineBell(t);
        }
    };
    
    // ---
    
    inline float expoBell(float t)
    {
        if (t < 0.5)
        {
            return ci::easeOutExpo(t * 2);
        }
        else
        {
            return 1 - ci::easeInExpo(t * 2 - 1);
        }
    }
    
    struct CurveExpoBell
    {
        float from;
        float to;
        
        CurveExpoBell(float from, float to) : from(from), to(to) {}
        
        float operator() (float t) const
        {
            return from + (to - from) * expoBell(t);
        }
    };
}

namespace chr = chronotext;
