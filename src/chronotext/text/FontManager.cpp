#include "chronotext/text/FontManager.h"
#include "chronotext/utils/Utils.h"

#include "cinder/DataSource.h"
#include "cinder/app/App.h"

#include <sstream>

using namespace ci;
using namespace app;
using namespace std;
using namespace chr;

FontManager::~FontManager()
{
    for (map<uint64_t, XFont*>::const_iterator it = cache.begin(); it != cache.end(); ++it)
    {
        delete it->second;
    }
    
    DLOG("FONTS DELETED: " << cache.size());
}

#if defined(CINDER_COCOA)
XFont* FontManager::getFont(const string &macPath, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
{
    stringstream oss;
    oss << macPath << useMipmap << useAnisotropy << maxDimensions << slotCapacity;
    
    string key = oss.str();
    uint64_t id = hash(key);
    
    if (hasFont(id))
    {
        return getFont(id);
    }
    else
    {
        DataSourceRef resource = loadResource(macPath);
        
        XFont *font = new XFont(resource, useMipmap, useAnisotropy, maxDimensions, slotCapacity);
        putFont(id, font);
        
        return font;
    }
}
#else
XFont* FontManager::getFont(int mswID, const string &mswType, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
{
    stringstream oss;
    oss << mswID << mswType << useMipmap << useAnisotropy << maxDimensions << slotCapacity;
    
    string key = oss.str();
    uint64_t id = hash(key);
    
    if (hasFont(id))
    {
        return getFont(id);
    }
    else
    {
        DataSourceRef resource = loadResource(mswID, mswType);
        
        XFont *font = new XFont(resource, useMipmap, useAnisotropy, maxDimensions, slotCapacity);
        putFont(id, font);
        
        return font;
    }
}
#endif

bool FontManager::removeFont(XFont *font)
{
    for (map<uint64_t, XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
    {
        if (font == it->second)
        {
            delete it->second;
            cache.erase(it);
            
            return true;
        }
    }
    
    return false;
}
