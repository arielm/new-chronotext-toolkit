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

#define EXCEPTION(HOST, ...) chr::Exception<HOST>(#HOST, __FILE__, __LINE__, ##__VA_ARGS__)
#define EXCEPTION_TYPE(HOST) chr::Exception<HOST>
