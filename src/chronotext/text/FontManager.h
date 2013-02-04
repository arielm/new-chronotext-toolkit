#pragma once

#include "chronotext/font/XFont.h"
#include "chronotext/utils/Hasher.h"

#include <map>

/*
 * TODO:
 * A BETTER VERSION SHOULD RETURN A "DERIVED" FONT WHENEVER POSSIBLE
 * E.G. WHEN ASKING FOR A FONT-ALREADY DEFINED, BUT WITH DIFFERENT PARAMETERS (useAnisotropy, maxDimensions OR slotCapacity)
 */

class FontManager
{
    std::map<uint64_t, chr::XFont*> cache;
    
    bool hasFont(uint64_t id)
    {
        return (cache.count(id) > 0);
    }
    
    chr::XFont* getFont(uint64_t id)
    {
        return cache[id];
    }
    
    void putFont(uint64_t id, chr::XFont *font)
    {
        cache[id] = font;
    }

public:
    ~FontManager();
    
#if defined(CINDER_COCOA)
    chr::XFont* getFont(const std::string &macPath, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
#else
    chr::XFont* getFont(int mswID, const std::string &mswType, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
#endif
    
    bool removeFont(chr::XFont *font);
};
