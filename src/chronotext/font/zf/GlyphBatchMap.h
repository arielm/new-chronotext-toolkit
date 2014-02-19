/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/GlyphBatch.h"
#include "chronotext/font/zf/ReloadableTexture.h"

#include <map>
#include <memory>

namespace chronotext
{
    namespace zf
    {
        class GlyphBatchMap
        {
        public:
            std::map<ReloadableTexture*, std::unique_ptr<GlyphBatch>> map;
            
            void clear()
            {
                for (auto &it : map)
                {
                    it.second->clear();
                }
            }
            
            GlyphBatch* getBatch(ReloadableTexture *texture)
            {
                auto it = map.find(texture);
                
                if (it == map.end())
                {
                    auto batch = new GlyphBatch;
                    map[texture] = std::unique_ptr<GlyphBatch>(batch);
                    return batch;
                }
                else
                {
                    return it->second.get();
                }
            }
            
            void flush(const GLushort *indices, bool useColor = false) const
            {
                for (auto &it : map)
                {
                    it.first->bind();
                    it.second->flush(indices, useColor);
                }
            }
        };
    }
}

namespace chr = chronotext;
