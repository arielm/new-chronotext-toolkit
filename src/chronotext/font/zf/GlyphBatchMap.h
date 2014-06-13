/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/GlyphBatch.h"
#include "chronotext/font/zf/FontTexture.h"

#include <map>
#include <memory>

namespace chronotext
{
    namespace zf
    {
        class GlyphBatchMap
        {
        public:
            std::map<FontTexture*, std::unique_ptr<GlyphBatch>> map;
            
            GlyphBatchMap() {}
            
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
                    if (it->second->size() == 0)
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
            
            GlyphBatch* getBatch(FontTexture *texture)
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
            
            void flush(const GLushort *indices, bool useColor = false, float anisotropy = 0) const
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
            
        private:
            GlyphBatchMap(const GlyphBatchMap &that); // MAKES IT EXPLICIT: GlyphBatchMap CAN'T BE COPIED (I.E. BECAUSE OF THE map OF unique_ptr)
        };
    }
}

namespace chr = chronotext;
