/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <exception>

#include "fmod_errors.h"

class FMODException : public std::exception
{
public:
    FMODException(FMOD_RESULT result) throw()
    :
    result(result)
    {}
    
    virtual const char* what() const throw()
    {
        return FMOD_ErrorString(result);
    }
    
protected:
    FMOD_RESULT result;
};
