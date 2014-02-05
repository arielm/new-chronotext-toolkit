/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/utils/Utils.h"

#include "utf8.h"

#include "boost/date_time/local_time/local_time.hpp"

using namespace std;
using namespace ci;
using namespace utf8;
using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;

namespace chronotext
{
    string wstringToUtf8(const wstring &s)
    {
        vector<char> tmp;
        
#if defined(CINDER_MSW)
        unchecked::utf16to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
        unchecked::utf32to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
        
        return string(tmp.data(), tmp.size());
    }
    
    wstring utf8ToWstring(const string &s)
    {
        vector<wchar_t> tmp;
        
#if defined(CINDER_MSW)
        unchecked::utf8to16(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
        unchecked::utf8to32(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
        
        return wstring(tmp.data(), tmp.size());
    }
    
    template<> string loadString<string>(InputSourceRef source)
    {
        Buffer buffer(source->loadDataSource());
        return string(static_cast<const char*>(buffer.getData()), buffer.getDataSize());
    }
    
    template<> wstring loadString<wstring>(InputSourceRef source)
    {
        return utf8ToWstring(loadString<string>(source));
    }
    
    template<> vector<std::string> readLines<string>(InputSourceRef source)
    {
        vector<string> lines;
        IStreamRef in = source->loadDataSource()->createStream();
        
        while (!in->isEof())
        {
            lines.push_back(in->readLine());
        }
        
        return lines;
    }
    
    template<> vector<wstring> readLines<wstring>(InputSourceRef source)
    {
        vector<wstring> lines;
        IStreamRef in = source->loadDataSource()->createStream();
        
        while (!in->isEof())
        {
            lines.push_back(utf8ToWstring(in->readLine()));
        }
        
        return lines;
    }
    
    vector<string> readInstructions(InputSourceRef source)
    {
        vector<string> lines = readLines<string>(source);
        vector<string> instructions;
        
        for (auto &line : lines)
        {
            boost::algorithm::trim(line);
            
            if (!line.empty() && !boost::starts_with(line, "#"))
            {
                instructions.emplace_back(line);
            }
        }
        
        return instructions;
    }
    
    // ---
    
    string hexDump(const char *data, int size)
    {
        stringstream s;
        s << hex;
        
        for (int i = 0; i < size; i++)
        {
            s << setfill('0') << setw(2) << (*data++ & 0xff) << " ";
        }
        
        return s.str();
    }
    
    /*
     * REFERENCE: http://stackoverflow.com/a/10096779/50335
     */
    string prettyBytes(uint64_t numBytes, int precision)
    {
        const char *abbrevs[] = { "TB", "GB", "MB", "KB" };
        const size_t numAbbrevs = sizeof(abbrevs) / sizeof(abbrevs[0]);
        
        uint64_t maximum = pow(1024, numAbbrevs);
        
        for (size_t i = 0; i < numAbbrevs; ++i)
        {
            if (numBytes > maximum)
            {
                stringstream s;
                s << fixed << setprecision(precision) << numBytes / (double)maximum << " " << abbrevs[i];
                return s.str();
            }
            
            maximum /= 1024;
        }
        
        stringstream s;
        s << numBytes << " Bytes";
        return s.str();
    }
    
    string percent(float ratio, int precision)
    {
        stringstream s;
        s << fixed << setprecision(precision) << ratio * 100 << "%";
        return s.str();
    }

    // ---
    
    uint64_t millisSinceEpoch()
    {
        ptime time_t_epoch(date(1970, 1, 1));
        ptime now = microsec_clock::local_time();
        time_duration diff = now - time_t_epoch;
        
        return diff.total_milliseconds();
    }
}
