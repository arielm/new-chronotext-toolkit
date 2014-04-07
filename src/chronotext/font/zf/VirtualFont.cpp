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

namespace chronotext
{
    namespace zf
    {
        VirtualFont::VirtualFont(FontManager &fontManager, const Properties &properties)
        :
        properties(properties),
        layoutCache(fontManager.layoutCache),
        itemizer(fontManager.itemizer),
        indices(fontManager.getIndices(properties.slotCapacity)),
        began(0),
        hasClip(false),
        sequence(nullptr)
        {
            assert(properties.baseSize > 0);
            assert(properties.slotCapacity > 0);
            assert(!(properties.useAnisotropy && !properties.useMipmap)); // ANISOTROPY DOESN'T MAKE SENSE WITHOUT MIPMAPS

            // ---
            
            if (gl::isExtensionAvailable("GL_EXT_texture_filter_anisotropic"))
            {
                if (properties.useAnisotropy)
                {
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
                }
                else
                {
                    anisotropy = 1;
                }
            }
            else
            {
                anisotropy = 0;
            }
            
            // ---

            setSize(properties.baseSize);
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
        
        float VirtualFont::getHeight(const LineLayout &layout) const
        {
            return layout.maxHeight * sizeRatio;
        }
        
        float VirtualFont::getAscent(const LineLayout &layout) const
        {
            return layout.maxAscent * sizeRatio;
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
        
        LineLayout* VirtualFont::createLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
        {
            TextLine line(text, langHint, overallDirection);
            itemizer.processLine(line);
            return createLineLayout(line, line.runs.cbegin(), line.runs.cend());
        }
        
        LineLayout* VirtualFont::createLineLayout(const TextLine &line, vector<TextRun>::const_iterator begin, vector<TextRun>::const_iterator end)
        {
            auto layout = new LineLayout(this, line.langHint, line.overallDirection);
            
            map<hb_codepoint_t, Cluster> clusterMap;
            auto buffer = hb_buffer_create();
            
            for (auto run = begin; run != end; ++run)
            {
                clusterMap.clear();
                
                for (auto &font : getFontSet(run->language))
                {
                    font->reload();
                    
                    if (font->loaded)
                    {
                        layout->maxHeight = std::max(layout->maxHeight, font->metrics.height);
                        layout->maxAscent = std::max(layout->maxAscent, font->metrics.ascent);
                        layout->maxDescent = std::max(layout->maxDescent, font->metrics.descent);
                        
                        run->apply(line.text, buffer);
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
                                    
                                    if (!properties.useMipmap)
                                    {
                                        offset.x = snap(offset.x);
                                        offset.y = snap(offset.y);
                                        advance = snap(advance);
                                    }
                                    
                                    if (clusterFound)
                                    {
                                        it->second.addShape(codepoint, offset, advance);
                                    }
                                    else
                                    {
                                        clusterMap.insert(make_pair(cluster, Cluster(font, run->tag, codepoint, offset, advance)));
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
                
                if (run->direction == HB_DIRECTION_RTL)
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
        
        void VirtualFont::preload(LineLayout &layout)
        {
            for (auto &cluster : layout.clusters)
            {
                for (auto &shape : cluster.shapes)
                {
                    auto glyph = shape.glyph = cluster.font->getGlyph(shape.codepoint);
                    
                    if (glyph && glyph->texture)
                    {
                        glyph->texture->reload(); // JUST IN CASE THE GLYPH HAVE BEEN PREVIOUSLY DISCARDED
                    }
                }
            }
        }
        
        void VirtualFont::setSize(float newSize)
        {
            size = newSize;
            sizeRatio = newSize / properties.baseSize;
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
        
        void VirtualFont::setClip(const Rectf &clipRect)
        {
            this->clipRect = clipRect;
            hasClip = true;
        }
        
        void VirtualFont::setClip(float x1, float y1, float x2, float y2)
        {
            clipRect.x1 = x1;
            clipRect.y1 = y1;
            clipRect.x2 = x2;
            clipRect.y2 = y2;
            
            hasClip = true;
        }
        
        void VirtualFont::clearClip()
        {
            hasClip = false;
        }
        
        FontMatrix* VirtualFont::getMatrix()
        {
            return &matrix;
        }
        
        const GLushort* VirtualFont::getIndices() const
        {
            return const_cast<GLushort*>(indices.data());
        }
        
        void VirtualFont::begin(bool useColor)
        {
            glEnable(GL_TEXTURE_2D);
            
            if (useColor)
            {
                glEnableClientState(GL_COLOR_ARRAY);
            }
            else
            {
                gl::color(color);
            }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        
        void VirtualFont::end(bool useColor)
        {
            glDisable(GL_TEXTURE_2D);
            
            if (useColor)
            {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        
        void VirtualFont::beginSequence(FontSequence *sequence, bool useColor)
        {
            if (began == 0)
            {
                sequenceUseColor = useColor;
                
                if (!batchMap)
                {
                    batchMap = unique_ptr<GlyphBatchMap>(new GlyphBatchMap);
                }
                else
                {
                    batchMap->clear();
                }
                
                if (sequence)
                {
                    this->sequence = sequence;
                    sequence->begin(useColor, anisotropy);
                }
                
                clearClip();
            }
            
            began++;
        }
        
        void VirtualFont::endSequence()
        {
            began--;
            
            if (began == 0)
            {
                if (sequence)
                {
                    batchMap->pack();
                    sequence->addMap(move(batchMap));
                    
                    sequence->end();
                    sequence = nullptr;
                }
                else
                {
                    begin(sequenceUseColor);
                    batchMap->flush(getIndices(), sequenceUseColor, anisotropy);
                    end(sequenceUseColor);
                }
            }
        }
        
        void VirtualFont::replaySequence(FontSequence *sequence)
        {
            begin(sequence->useColor);
            sequence->replay(getIndices());
            end(sequence->useColor);
        }
        
        void VirtualFont::incrementSequence(GlyphBatch *batch)
        {
            if (sequenceUseColor)
            {
                batch->addColor(color);
            }
            
            if (batch->size() == properties.slotCapacity)
            {
                if (sequence)
                {
                    batchMap->pack();
                    sequence->addMap(move(batchMap));
                }
                else
                {
                    begin(sequenceUseColor);
                    batchMap->flush(getIndices(), sequenceUseColor, anisotropy);
                    end(sequenceUseColor);
                }
                
                if (!batchMap)
                {
                    batchMap = unique_ptr<GlyphBatchMap>(new GlyphBatchMap);
                }
                else
                {
                    batchMap->clear();
                }
            }
        }
        
        bool VirtualFont::clipQuad(GlyphQuad &quad, FontTexture *texture) const
        {
            return quad.clip(clipRect, texture->getSize() * sizeRatio);
        }

        void VirtualFont::addCluster(const Cluster &cluster, const Vec3f &position)
        {
            for (auto &shape : cluster.shapes)
            {
                Vec2f p = properties.useMipmap ? position.xy() : Vec2f(snap(position.x), snap(position.y));
                
                GlyphQuad quad;
                auto glyph = cluster.font->fillQuad(quad, shape, p, sizeRatio);
                
                if (glyph)
                {
                    if (!hasClip || clipQuad(quad, glyph->texture))
                    {
                        auto batch = batchMap->getBatch(glyph->texture);
                        batch->addQuad(quad, position.z);
                        incrementSequence(batch);
                    }
                }
            }
        }
        
        void VirtualFont::addTransformedCluster(const Cluster &cluster, const Vec2f &position)
        {
            for (auto &shape : cluster.shapes)
            {
                GlyphQuad quad;
                auto glyph = cluster.font->fillQuad(quad, shape, position, sizeRatio);
                
                if (glyph)
                {
                    if (!hasClip || clipQuad(quad, glyph->texture))
                    {
                        auto batch = batchMap->getBatch(glyph->texture);
                        matrix.addTransformedQuad(quad, batch->vertices);
                        incrementSequence(batch);
                    }
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
        
        float VirtualFont::snap(float value)
        {
            if (value < 0)
            {
                return math<float>::ceil(value - 0.5f);
            }
            else
            {
                return math<float>::floor(value + 0.5f);
            }
        }
    }
}
