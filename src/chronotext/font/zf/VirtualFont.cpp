/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/VirtualFont.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace zf
    {
        VirtualFont::VirtualFont(LayoutCache &layoutCache, TextItemizer &itemizer, float baseSize)
        :
        layoutCache(layoutCache),
        itemizer(itemizer),
        baseSize(baseSize)
        {
            vertices.reserve(4 * 2);
            colors.reserve(4);
            
            // ---
            
            indices.reserve(6);
            
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(2);
            indices.push_back(3);
            indices.push_back(0);
            
            // ---
            
            setSize(baseSize);
            setColor(ColorA(0, 0, 0, 1));
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
        
        void VirtualFont::setColor(const ColorA &color)
        {
            colors.clear();
            colors.emplace_back(color);
            colors.emplace_back(color);
            colors.emplace_back(color);
            colors.emplace_back(color);
        }
        
        void VirtualFont::begin()
        {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            
            const int stride = sizeof(Vec2f) * 2;
            
            glVertexPointer(2, GL_FLOAT, stride, vertices.data());
            glTexCoordPointer(2, GL_FLOAT, stride, vertices.data() + 1);
            glColorPointer(4, GL_FLOAT, 0, colors.data());
        }
        
        void VirtualFont::end()
        {
            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
        }
        
        void VirtualFont::drawCluster(const Cluster &cluster, const Vec2f &position)
        {
            for (auto &shape : cluster.shapes)
            {
                GlyphQuad quad;
                ActualFont::Glyph *glyph;
                tie(quad, glyph) = obtainQuad(cluster, shape, position);
                
                if (glyph)
                {
                    vertices.clear();
                    addQuad(quad);
                    
                    glyph->texture->bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices.data());
                }
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
        
        pair<GlyphQuad, ActualFont::Glyph*> VirtualFont::obtainQuad(const Cluster &cluster, const Shape &shape, const Vec2f &position) const
        {
            GlyphQuad quad;
            auto glyph = cluster.font->getGlyph(shape.codepoint);
            
            if (glyph)
            {
                if (glyph->texture)
                {
                    auto ul = position + (shape.position + glyph->offset) * sizeRatio;
                    
                    quad.x1 = ul.x;
                    quad.y1 = ul.y,
                    quad.x2 = ul.x + glyph->size.x * sizeRatio;
                    quad.y2 = ul.y + glyph->size.y * sizeRatio;
                    
                    quad.u1 = glyph->u1;
                    quad.v1 = glyph->v1;
                    quad.u2 = glyph->u2;
                    quad.v2 = glyph->v2;
                }
                else
                {
                    glyph = NULL;
                }
            }
            
            return make_pair(quad, glyph);
        }
        
        void VirtualFont::addQuad(const GlyphQuad &quad)
        {
            vertices.emplace_back(quad.x1, quad.y1);
            vertices.emplace_back(quad.u1, quad.v1);
            
            vertices.emplace_back(quad.x2, quad.y1);
            vertices.emplace_back(quad.u2, quad.v1);
            
            vertices.emplace_back(quad.x2, quad.y2);
            vertices.emplace_back(quad.u2, quad.v2);
            
            vertices.emplace_back(quad.x1, quad.y2);
            vertices.emplace_back(quad.u1, quad.v2);
        }
    }
}
