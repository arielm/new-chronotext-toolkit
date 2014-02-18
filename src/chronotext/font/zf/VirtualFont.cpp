/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/VirtualFont.h"
#include "chronotext/font/zf/FontManager.h"

using namespace std;
using namespace ci;

#define SLOT_CAPACITY 1024 /* FIXME: TEMPORARY (SEE TARGET FUNCTIONALITY IN XFont) */

namespace chronotext
{
    namespace zf
    {
        VirtualFont::VirtualFont(FontManager &fontManager, float baseSize)
        :
        baseSize(baseSize),
        layoutCache(fontManager.layoutCache),
        itemizer(fontManager.itemizer),
        indices(fontManager.getIndices(SLOT_CAPACITY))
        {
            setSize(baseSize);
            setColor(0, 0, 0, 1);
        }
        
        bool VirtualFont::addActualFont(const string &lang, ActualFont *font)
        {
            if (font)
            {
                /*
                 * WHY USING std::vector INSTEAD OF std::set?
                 * BECAUSE ORDER OF INSERTION MATTERS
                 */
                
                for (auto &tmp : fontSetMap[lang])
                {
                    if (tmp == font)
                    {
                        return false;
                    }
                }
                
                fontSetMap[lang].push_back(font);
                return true;
            }
            
            return false;
        }
        
        const FontSet& VirtualFont::getFontSet(const string &lang) const
        {
            auto it = fontSetMap.find(lang);
            
            if (it == fontSetMap.end())
            {
                it = fontSetMap.find("");
                
                if (it == fontSetMap.end())
                {
                    return defaultFontSet;
                }
            }
            
            return it->second;
        }
        
        ActualFont::Metrics VirtualFont::getMetrics(const Cluster &cluster) const
        {
            return cluster.font->metrics * sizeRatio;
        }
        
        ActualFont::Metrics VirtualFont::getMetrics(const string &lang) const
        {
            auto &fontSet = getFontSet(lang);
            
            if (fontSet.empty())
            {
                return ActualFont::Metrics();
            }
            else
            {
                return fontSet.front()->metrics * sizeRatio;
            }
        }
        
        float VirtualFont::getDescent(const LineLayout &layout) const
        {
            return layout.maxDescent * sizeRatio;
        }
        
        float VirtualFont::getMiddleLine(const LineLayout &layout) const
        {
            return 0.5f * (layout.maxAscent - layout.maxDescent) * sizeRatio;
        }
        
        float VirtualFont::getOffsetX(const LineLayout &layout, Alignment align) const
        {
            switch (align)
            {
                case ALIGN_MIDDLE:
                    return -0.5f * getAdvance(layout);
                    
                case ALIGN_RIGHT:
                    return -getAdvance(layout);
                    
                default:
                    return 0;
            }
        }
        
        float VirtualFont::getOffsetY(const LineLayout &layout, Alignment align) const
        {
            switch (align)
            {
                case ALIGN_MIDDLE:
                    return getMiddleLine(layout);
                    
                case ALIGN_TOP:
                    return +getAscent(layout);
                    
                case ALIGN_BOTTOM:
                    return -getDescent(layout);
                    
                default:
                    return 0;
            }
        }
        
        float VirtualFont::getAdvance(const LineLayout &layout) const
        {
            return layout.advance * sizeRatio;
        }
        
        float VirtualFont::getAdvance(const Cluster &cluster) const
        {
            return cluster.combinedAdvance * sizeRatio;
        }
        
        float VirtualFont::getHeight(const LineLayout &layout) const
        {
            return layout.maxHeight * sizeRatio;
        }
        
        float VirtualFont::getAscent(const LineLayout &layout) const
        {
            return layout.maxAscent * sizeRatio;
        }
        
        LineLayout* VirtualFont::createLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
        {
            return createLineLayout(itemizer.processLine(text, langHint, overallDirection));
        }
        
        LineLayout* VirtualFont::createLineLayout(const TextLine &line)
        {
            auto layout = new LineLayout(this, line.langHint, line.overallDirection);
            
            map<uint32_t, Cluster> clusterMap;
            auto buffer = hb_buffer_create();
            
            for (auto &run : line.runs)
            {
                clusterMap.clear();
                
                for (auto &font : getFontSet(run.language))
                {
                    font->reload();
                    
                    if (font->loaded)
                    {
                        layout->maxHeight = std::max(layout->maxHeight, font->metrics.height);
                        layout->maxAscent = std::max(layout->maxAscent, font->metrics.ascent);
                        layout->maxDescent = std::max(layout->maxDescent, font->metrics.descent);
                        
                        run.apply(line.text, buffer);
                        hb_shape(font->hbFont, buffer, NULL, 0);
                        
                        auto glyphCount = hb_buffer_get_length(buffer);
                        auto glyphInfos = hb_buffer_get_glyph_infos(buffer, NULL);
                        auto glyphPositions = hb_buffer_get_glyph_positions(buffer, NULL);
                        
                        bool hasMissingGlyphs = false;
                        
                        for (int i = 0; i < glyphCount; i++)
                        {
                            auto codepoint = glyphInfos[i].codepoint;
                            auto cluster = glyphInfos[i].cluster;
                            
                            auto it = clusterMap.find(cluster);
                            bool clusterFound = (it != clusterMap.end());
                            
                            if (codepoint)
                            {
                                if (clusterFound && (it->second.font != font))
                                {
                                    continue; // CLUSTER FOUND, WITH ANOTHER FONT (E.G. SPACE)
                                }
                                else
                                {
                                    auto offset = Vec2f(glyphPositions[i].x_offset, -glyphPositions[i].y_offset) * font->scale;
                                    float advance = glyphPositions[i].x_advance * font->scale.x;
                                    
                                    if (clusterFound)
                                    {
                                        it->second.addShape(codepoint, offset, advance);
                                    }
                                    else
                                    {
                                        clusterMap.insert(make_pair(cluster, Cluster(font, codepoint, offset, advance)));
                                    }
                                }
                            }
                            else if (!clusterFound)
                            {
                                hasMissingGlyphs = true;
                            }
                        }
                        
                        if (!hasMissingGlyphs)
                        {
                            break; // NO NEED TO PROCEED TO THE NEXT FONT IN THE LIST
                        }
                    }
                }
                
                if (run.direction == HB_DIRECTION_RTL)
                {
                    for (auto it = clusterMap.rbegin(); it != clusterMap.rend(); ++it)
                    {
                        layout->addCluster(it->second);
                    }
                }
                else
                {
                    for (auto it = clusterMap.begin(); it != clusterMap.end(); ++it)
                    {
                        layout->addCluster(it->second);
                    }
                }
            }
            
            hb_buffer_destroy(buffer);
            return layout;
        }
        
        shared_ptr<LineLayout> VirtualFont::getCachedLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
        {
            return layoutCache.getLineLayout(this, text, langHint, overallDirection);
        }
        
        void VirtualFont::setSize(float newSize)
        {
            size = newSize;
            sizeRatio = newSize / baseSize;
        }
        
        void VirtualFont::setColor(const ColorA &newColor)
        {
            color = newColor;
        }
        
        void VirtualFont::setColor(float r, float g, float b, float a)
        {
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }
        
        void VirtualFont::begin()
        {
            for (auto &it : fontSequence)
            {
                it.second->clear();
            }
        }
        
        void VirtualFont::end()
        {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            
            for (auto &it : fontSequence)
            {
                it.first->bind();
                it.second->flush(indices);
            }

            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
        }
        
        void VirtualFont::addCluster(const Cluster &cluster, const Vec2f &position)
        {
            for (auto &shape : cluster.shapes)
            {
                GlyphQuad quad;
                ActualFont::Glyph *glyph;
                tie(quad, glyph) = cluster.font->obtainQuad(shape, position, sizeRatio);
                
                if (glyph)
                {
                    auto glyphSequence = getGlyphSequence2D(glyph->texture);
                    glyphSequence->addQuad(quad);
                    glyphSequence->addColor(color);
                }
            }
        }
        
        GlyphSequence2D* VirtualFont::getGlyphSequence2D(ReloadableTexture *texture)
        {
            auto it = fontSequence.find(texture);
            
            if (it == fontSequence.end())
            {
                auto glyphSequence = new GlyphSequence2D;
                fontSequence[texture] = unique_ptr<GlyphSequence2D>(glyphSequence);
                return glyphSequence;
            }
            else
            {
                return it->second.get();
            }
        }
        
        VirtualFont::Style VirtualFont::styleStringToEnum(const string &style)
        {
            if (style == "bold") return VirtualFont::STYLE_BOLD;
            if (style == "italic") return VirtualFont::STYLE_ITALIC;
            if (style == "bold-italic") return VirtualFont::STYLE_BOLD_ITALIC;
            
            return VirtualFont::STYLE_REGULAR; // DEFAULT
        }
        
        string VirtualFont::styleEnumToString(VirtualFont::Style style)
        {
            switch (style)
            {
                case VirtualFont::STYLE_BOLD:
                    return "bold";
                    
                case VirtualFont::STYLE_ITALIC:
                    return "italic";
                    
                case VirtualFont::STYLE_BOLD_ITALIC:
                    return "bold-italic";
                    
                default:
                case VirtualFont::STYLE_REGULAR:
                    return "regular";
            }
        }
    }
}
