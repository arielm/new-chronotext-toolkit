/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/zf/ActualFont.h"
#include "chronotext/font/zf/LayoutCache.h"
#include "chronotext/font/zf/TextItemizer.h"
#include "chronotext/font/zf/FontSequence.h"

#include <set>
#include <map>

namespace chronotext
{
    typedef class zf::VirtualFont ZFont;

    namespace zf
    {
        class FontManager;
        typedef std::vector<ActualFont*> FontSet;
        
        class VirtualFont
        {
        public:
            typedef enum
            {
                STYLE_REGULAR,
                STYLE_BOLD,
                STYLE_ITALIC,
                STYLE_BOLD_ITALIC
            }
            Style;
            
            typedef enum
            {
                ALIGN_MIDDLE,
                ALIGN_LEFT,
                ALIGN_RIGHT,
                ALIGN_TOP,
                ALIGN_BASELINE,
                ALIGN_BOTTOM
            }
            Alignment;
            
            struct Properties
            {
                float baseSize;
                bool useMipmap;
                bool useAnisotropy;
                int slotCapacity;
                
                Properties(float baseSize, bool useMipmap, bool useAnisotropy, int slotCapacity)
                :
                baseSize(baseSize),
                useMipmap(useMipmap),
                useAnisotropy(useAnisotropy),
                slotCapacity(slotCapacity)
                {}
                
                Properties& setCapacity(float slotCapacity)
				{
                    this->slotCapacity = slotCapacity;
                    return *this;
                }
                
                Properties& setCrisp()
				{
                    useMipmap = false;
                    return *this;
                }
                
                bool operator<(const Properties &rhs) const
                {
                    return std::tie(baseSize, useMipmap, useAnisotropy, slotCapacity) < std::tie(rhs.baseSize, rhs.useMipmap, rhs.useAnisotropy, rhs.slotCapacity);
                }
            };
            
            static Properties Properties2d(float baseSize = 0)
            {
                return Properties(baseSize, true, false, 1024);
            }
            
            static Properties Properties3d(float baseSize = 0)
            {
                return Properties(baseSize, true, true, 1024);
            }
            
            LayoutCache &layoutCache;
            TextItemizer &itemizer;
            
            ActualFont::Metrics getMetrics(const Cluster &cluster) const; // RETURNS THE SIZED METRICS OF THE ActualFont USED BY cluster
            ActualFont::Metrics getMetrics(const std::string &lang = "") const; // RETURNS THE SIZED METRICS OF THE FIRST ActualFont IN THE SET USED FOR lang
            
            float getHeight(const LineLayout &layout) const;
            float getAscent(const LineLayout &layout) const;
            float getDescent(const LineLayout &layout) const;
            float getMiddleLine(const LineLayout &layout) const;
            
            float getOffsetX(const LineLayout &layout, Alignment align) const;
            float getOffsetY(const LineLayout &layout, Alignment align) const;
            inline ci::Vec2f getOffset(const LineLayout &layout, Alignment alignX, Alignment alignY) const { return ci::Vec2f(getOffsetX(layout, alignX), getOffsetY(layout, alignY)); }
            
            float getAdvance(const LineLayout &layout) const;
            float getAdvance(const Cluster &cluster) const;
            
            /*
             * THE RETURNED INSTANCES ARE NOT MANAGED AND SHOULD BE DELETED BY THE CALLER
             */
            LineLayout* createLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            LineLayout* createLineLayout(const TextLine &line, std::vector<TextRun>::const_iterator begin, std::vector<TextRun>::const_iterator end);
            
            std::shared_ptr<LineLayout> getCachedLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            
            /*
             * - WILL LOAD THE GLYPH TEXTURES IMMEDIATELY (I.E. INSTEAD OF "JUST BEFORE DRAWING")
             * - WILL SAVE ONE std::map<uint32_t> LOOKUP PER addCluster() OR addTransformedCluster()
             */
            void preload(LineLayout &layout);
            
            void setSize(float size);
            void setColor(const ci::ColorA &color);
            void setColor(float r, float g, float b, float a);
            
            void setClip(const ci::Rectf &clipRect);
            void setClip(float x1, float y1, float x2, float y2);
            void clearClip();
            
            FontMatrix* getMatrix();
            const GLushort* getIndices() const;
            
            void beginSequence(FontSequence *sequence, bool useColor = false);
            inline void beginSequence(FontSequence &sequence, bool useColor = false) { beginSequence(&sequence, useColor); }
            inline void beginSequence(bool useColor = false) { beginSequence(nullptr, useColor); }
            void endSequence();
            void replaySequence(FontSequence *sequence);
            inline void replaySequence(FontSequence &sequence) { replaySequence(&sequence); }
            
            void addCluster(const Cluster &cluster, const ci::Vec3f &position);
            inline void addCluster(const Cluster &cluster, const ci::Vec2f &position) { addCluster(cluster, ci::Vec3f(position)); }
            inline void addCluster(const Cluster &cluster, float x, float y, float z = 0) { addCluster(cluster, ci::Vec3f(x, y, z)); }
            void addTransformedCluster(const Cluster &cluster, const ci::Vec2f &position);
            inline void addTransformedCluster(const Cluster &cluster, float x, float y) { addTransformedCluster(cluster, ci::Vec2f(x, y)); }
            
            static Style styleStringToEnum(const std::string &style);
            static std::string styleEnumToString(Style style);
            static float snap(float value);
            
            friend class FontManager;
            
        protected:
            float size;
            float sizeRatio;
            ci::ColorA color;
            
            bool hasClip;
            ci::Rectf clipRect;

            Properties properties;
            const std::vector<GLushort> &indices;
            FontMatrix matrix;
            
            float anisotropy;
            
            int began;
            bool sequenceUseColor;
            FontSequence *sequence;
            std::unique_ptr<GlyphBatchMap> batchMap;
            
            FontSet defaultFontSet; // ALLOWING getFontSet() TO RETURN CONST VALUES
            std::map<std::string, FontSet> fontSetMap;
            
            VirtualFont(FontManager &fontManager, const Properties &properties);
            
            bool addActualFont(const std::string &lang, ActualFont *font);
            const FontSet& getFontSet(const std::string &lang) const;
            
            void begin(bool useColor = false);
            void end(bool useColor = false);
            
            void incrementSequence(GlyphBatch *batch);
            bool clipQuad(GlyphQuad &quad, FontTexture *texture) const;
        };
    }
}

namespace chr = chronotext;
