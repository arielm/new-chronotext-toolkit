/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/utils/Utils.h"

#include "utf8.h"

using namespace std;
using namespace ci;
using namespace utf8;

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
    
    string loadUtf8(DataSourceRef source)
    {
        Buffer buffer(source);
        return string(static_cast<const char*>(buffer.getData()), buffer.getDataSize());
    }
    
    wstring loadWstring(DataSourceRef source)
    {
        return utf8ToWstring(loadUtf8(source));
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

}
