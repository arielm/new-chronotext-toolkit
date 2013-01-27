#pragma once

namespace chronotext
{
    class TimeBase
    {
    public:
        virtual ~TimeBase() {}
        virtual double getTime() = 0;
    };
}

namespace chr = chronotext;
