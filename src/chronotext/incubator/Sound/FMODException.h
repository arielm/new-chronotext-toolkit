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
