#pragma once

#include <stdexcept>
#include <sstream>

namespace chronotext
{
    template<class Host>
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &type = "", const std::string &file = "", int line = 0, const std::string &what = "") throw()
        :
        std::runtime_error(what),
        what(what),
        type(type),
        file(file),
        line(line)
        {}
        
        ~Exception() throw() {}
        
        const std::string message(bool detailed = false)
        {
            std::stringstream buffer;
            
            if (!type.empty())
            {
                buffer << type << "::";
            }
            
            buffer << "Exception";
            
            if (!what.empty())
            {
                buffer << ": " << what;
            }
            
            if (detailed)
            {
                if (!file.empty())
                {
                    buffer << " [" << file;
                    
                    if (line > 0)
                    {
                        buffer << " | LINE: " << line;
                    }
                    
                    buffer << "]";
                }
            }
            
            return buffer.str();
        }
        
    protected:
        std::string what;
        std::string type;
        std::string file;
        int line;
    };
}

namespace chr = chronotext;

#define EXCEPTION(HOST, ...) chr::Exception<HOST>(#HOST, __FILE__, __LINE__, ##__VA_ARGS__)
#define EXCEPTION_TYPE(HOST) chr::Exception<HOST>
