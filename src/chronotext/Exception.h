#pragma once

#include <stdexcept>

namespace chronotext
{
    template<class Host>
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &what) : std::runtime_error(what) {}
    };
}

namespace chr = chronotext;
