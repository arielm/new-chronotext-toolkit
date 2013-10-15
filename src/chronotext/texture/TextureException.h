#pragma once

#include <exception>

class TextureException : public std::exception
{
public:
    TextureException() throw()
    {}
    
    TextureException(const char *what) throw()
    :
    message(what)
    {}
    
    virtual const char* what() const throw()
    {
        return message;
    }
    
protected:
    const char *message;
};
