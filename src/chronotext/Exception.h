/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <stdexcept>
#include <sstream>

namespace chronotext
{
    template<typename T>
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &type = "", const std::string &file = "", int line = 0, const std::string &what = "") throw()
        :
        runtime_error(what),
        type(type),
        file(file),
        line(line)
        {}
        
        ~Exception() throw() {}
        
        const std::string& getType() const
        {
            return type;
        }
        
        const std::string& getFile() const
        {
            return file;
        }
        
        int getLine() const
        {
            return line;
        }
        
        /*
         * TODO: POSSIBILITY TO CONTROL THE LEVEL-OF-DETAILS
         *
         * OPTION 1: VIA "CUSTOM STREAM MANIPULATORS" (SEE: std::iomanip, setw, setFill. ETC.)
         * OPTION 2: BY QUERYING THE std::ostream (POSSIBLY CASTABLE TO SOME FRIENDLY CLASS)
         */
        friend std::ostream& operator<<(std::ostream &lhs, const Exception &rhs)
        {
            if (!rhs.type.empty())
            {
                lhs << rhs.type << "::";
            }
            
            lhs << "Exception";
            
            if (rhs.what())
            {
                lhs << ": " << rhs.what();
            }
            
            if (false)
            {
                if (!rhs.file.empty())
                {
                    lhs << " [" << rhs.file;
                    
                    if (rhs.line > 0)
                    {
                        lhs << " | LINE: " << rhs.line;
                    }
                    
                    lhs << "]";
                }
            }
            
            return lhs;
        }
        
    protected:
        std::string type;
        std::string file;
        int line;
    };
}

namespace chr = chronotext;

#define EXCEPTION(TYPE, ...) chr::Exception<TYPE>(#TYPE, __FILE__, __LINE__, ##__VA_ARGS__)
#define EXCEPTION_TYPE(TYPE) chr::Exception<TYPE>
