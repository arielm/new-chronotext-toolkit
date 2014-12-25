/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadBatch.h"

#include <map>
#include <memory>

namespace chr
{
    namespace zf
    {
        template<typename T> class QuadBatchMap
        {
        public:
            std::map<T*, std::unique_ptr<QuadBatch>> map;
            
            QuadBatchMap() {}
            QuadBatchMap(const QuadBatchMap &other) = delete; // QuadBatchMap CAN'T BE COPIED (I.E. BECAUSE OF THE map OF unique_ptr)
            
            void clear()
            {
                for (auto &it : map)
                {
                    it.second->clear();
                }
            }
            
            void pack()
            {
                for (auto it = map.begin(); it != map.end();)
                {
                    if (it->second->empty())
                    {
                        it = map.erase(it);
                    }
                    else
                    {
                        it->second->pack();
                        ++it;
                    }
                }
            }
            
            QuadBatch* getBatch(T *texture)
            {
                auto it = map.find(texture);
                
                if (it == map.end())
                {
                    auto batch = new QuadBatch;
                    map[texture] = std::unique_ptr<QuadBatch>(batch);
                    return batch;
                }
                else
                {
                    return it->second.get();
                }
            }
            
            void flush(const uint16_t *indices, bool useColor = false, float anisotropy = 0) const
            {
                for (auto &it : map)
                {
                    it.first->bind(); // RELOADS THE TEXTURE, IF NECESSARY

                    if (anisotropy)
                    {
                        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
                    }

                    it.second->flush(indices, useColor);
                }
            }
        };
    }
}
