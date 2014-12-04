/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON MurmurHash2_64 BY Austin Appleby: https://sites.google.com/site/murmurhash
 * THE MurmurHash64B VARIANT USED HERE IS PRODUCING A 64-BIT HASH FOR 32-BIT PLATFORMS
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace chr
{
    static uint64_t hash(const void *key, int len, unsigned int seed)
    {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
        
        unsigned int h1 = seed ^ len;
        unsigned int h2 = 0;
        
        const unsigned int *data = (const unsigned int*)key;
        
        while (len >= 8)
        {
            unsigned int k1 = *data++;
            k1 *= m; k1 ^= k1 >> r; k1 *= m;
            h1 *= m; h1 ^= k1;
            len -= 4;
            
            unsigned int k2 = *data++;
            k2 *= m; k2 ^= k2 >> r; k2 *= m;
            h2 *= m; h2 ^= k2;
            len -= 4;
        }
        
        if (len >= 4)
        {
            unsigned int k1 = *data++;
            k1 *= m; k1 ^= k1 >> r; k1 *= m;
            h1 *= m; h1 ^= k1;
            len -= 4;
        }
        
        switch (len)
        {
            case 3: h2 ^= ((unsigned char*)data)[2] << 16;
            case 2: h2 ^= ((unsigned char*)data)[1] << 8;
            case 1: h2 ^= ((unsigned char*)data)[0];
                h2 *= m;
        };
        
        h1 ^= h2 >> 18; h1 *= m;
        h2 ^= h1 >> 22; h2 *= m;
        h1 ^= h2 >> 17; h1 *= m;
        h2 ^= h1 >> 19; h2 *= m;
        
        uint64_t h = h1;
        h = (h << 32) | h2;
        
        return h;
    }
    
    static uint64_t hash(const std::string &value, unsigned int seed = 0)
    {
        return hash((void*)value.data(), value.size(), seed);
    }
}
