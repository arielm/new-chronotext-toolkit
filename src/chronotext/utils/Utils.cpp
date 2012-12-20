#include "chronotext/utils/Utils.h"

#include "utf8.h"

using namespace std;
using namespace utf8;

string wstringToUtf8(const wstring &s)
{
    vector<char> tmp;

#if defined(CINDER_MSW)
    unchecked::utf16to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
    unchecked::utf32to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
    
    return string(&tmp[0], tmp.size());
}

wstring utf8ToWstring(const string &s)
{
    vector<wchar_t> tmp;

#if defined(CINDER_MSW)
    unchecked::utf8to16(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
    unchecked::utf8to32(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
    
    return wstring(&tmp[0], tmp.size());
}
