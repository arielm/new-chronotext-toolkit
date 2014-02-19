/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/ActualFont.h"
#include "chronotext/font/zf/LayoutCache.h"
#include "chronotext/font/zf/TextItemizer.h"
#include "chronotext/font/FontMatrix.h"

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
                {
                    assert(baseSize >= 0);
                    assert((slotCapacity > 0) && (slotCapacity <= 8192));
                    assert(!(useAnisotropy && !useMipmap)); // ANISOTROPY DOESN'T MAKE SENSE WITHOUT MIPMAPS
                    assert(!((baseSize == 0) && !useMipmap)); // FOR CRISP-RENDERING: A FONT-SIZE MUST BE DEFINED AND MIPMAPS DISABLED
                }
                
                bool operator<(const Properties &rhs) const
                {
                    return std::tie(baseSize, useMipmap, useAnisotropy, slotCapacity) < std::tie(rhs.baseSize, rhs.useMipmap, rhs.useAnisotropy, rhs.slotCapacity);
                }
            };
            
            static Properties Properties2d(float baseSize = 0, int slotCapacity = 1024)
            {
                return Properties(baseSize, true, false, slotCapacity);
            }
            
            static Properties Properties3d(float baseSize = 0, int slotCapacity = 1024)
            {
                return Properties(baseSize, true, true, slotCapacity);
            }
            
            static Properties PropertiesCrisp(float baseSize, int slotCapacity = 1024)
            {
                return Properties(baseSize, false, false, slotCapacity);
            }
            
            LayoutCache &layoutCache;
            TextItemizer &itemizer;
            float baseSize;
            
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
            LineLayout* createLineLayout(const TextLine &line);
            
            std::shared_ptr<LineLayout> getCachedLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            
            void setSize(float size);
            void setColor(const ci::ColorA &color);
            void setColor(float r, float g, float b, float a);
            
            void begin();
            void end();
            
            void addCluster(const Cluster &cluster, const ci::Vec2f &position);
            
            static Style styleStringToEnum(const std::string &style);
            static std::string styleEnumToString(Style style);
            
            friend class FontManager;
            
        protected:
            float size;
            float sizeRatio;
            ci::ColorA color;

            Properties properties;
            const std::vector<GLushort> &indices;
            
            std::map<ReloadableTexture*, std::unique_ptr<GlyphSequence>> fontSequence;
            
            FontSet defaultFontSet; // ALLOWING getFontSet() TO RETURN CONST VALUES
            std::map<std::string, FontSet> fontSetMap;
            
            VirtualFont(FontManager &fontManager, const Properties &properties);
            
            bool addActualFont(const std::string &lang, ActualFont *font);
            const FontSet& getFontSet(const std::string &lang) const;
            
            GlyphSequence* getGlyphSequence(ReloadableTexture *texture);
        };
    }
}

namespace chr = chronotext;
