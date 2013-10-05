/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Exception.h"

#include "fmod_errors.h"

class FMODException : public ci::Exception
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
