/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadMatrix.h"
#include "chronotext/font/zf/ActualFont.h"
#include "chronotext/font/zf/FontSequence.h"
#include "chronotext/font/zf/LineLayout.h"
#include "chronotext/font/zf/TextItemizer.h"

#include <boost/range/iterator_range.hpp>

#include <set>
#include <map>

namespace chr
{
    typedef class zf::VirtualFont ZFont;

    namespace zf
    {
        class FontManager;
        class LayoutStore;

        typedef std::vector<ActualFont*> FontSet;
        
        class VirtualFont
        {
        public:
            enum Style
            {
                STYLE_REGULAR,
                STYLE_BOLD,
                STYLE_ITALIC,
                STYLE_BOLD_ITALIC
            };
            
            enum Alignment
            {
                ALIGN_MIDDLE,
                ALIGN_LEFT,
                ALIGN_RIGHT,
                ALIGN_TOP,
                ALIGN_BASELINE,
                ALIGN_BOTTOM
            };
            
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
                    Properties::slotCapacity = slotCapacity;
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
            
            std::shared_ptr<LayoutStore> layoutStore;
            std::shared_ptr<TextItemizer> itemizer;
            
            ActualFont::Metrics getMetrics(const Cluster &cluster) const; // RETURNS THE SIZED METRICS OF THE ActualFont USED BY cluster
            
            /*
             * METRICS TAKE IN COUNT EVERY ActualFont INCLUDED IN THE "COMPOSITE LINE"
             */
            float getHeight(const LineLayout &layout) const; // RETURNS THE LINE'S MAXIMUM
            float getAscent(const LineLayout &layout) const; // RETURNS THE LINE'S MAXIMUM
            float getDescent(const LineLayout &layout) const; // RETURNS THE LINE'S MAXIMUM
            float getLineThickness(const LineLayout &layout) const; // RETURNS THE LINE'S MAXIMUM
            float getUnderlineOffset(const LineLayout &layout) const; // RETURNS THE LINE'S MAXIMUM
            float getStrikethroughOffset(const LineLayout &layout) const; // RETURNS THE LINE'S AVERAGE
            
            float getOffsetX(const LineLayout &layout, Alignment align) const;
            float getOffsetY(const LineLayout &layout, Alignment align) const; // FOR "ALIGN_MIDDLE": getStrikethroughOffset() WILL BE USED, UNLESS setMiddleLineFactor() HAS BEEN INVOKED
            inline ci::Vec2f getOffset(const LineLayout &layout, Alignment alignX, Alignment alignY) const { return ci::Vec2f(getOffsetX(layout, alignX), getOffsetY(layout, alignY)); }
            
            float getAdvance(const LineLayout &layout) const;
            float getAdvance(const Cluster &cluster) const;
            
            /*
             * THE RETURNED INSTANCES ARE NOT MANAGED AND SHOULD BE DELETED BY THE CALLER
             */
            LineLayout* createLineLayout(const std::string &text, hb_language_t = HB_LANGUAGE_INVALID, hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            LineLayout* createLineLayout(const TextLine &line, boost::iterator_range<std::list<TextRun>::const_iterator> range);
            
            /*
             * THE RETURNED INSTANCES ARE MANAGED BY THE FontManager's LayoutStore
             */
            std::shared_ptr<LineLayout> getLineLayout(const std::string &text, hb_language_t = HB_LANGUAGE_INVALID, hb_direction_t overallDirection = HB_DIRECTION_INVALID);
            
            /*
             * - WILL LOAD THE GLYPH TEXTURES IMMEDIATELY (I.E. INSTEAD OF "JUST BEFORE DRAWING")
             * - WILL SAVE ONE std::map<uint32_t> LOOKUP PER addCluster() OR addTransformedCluster()
             */
            void preload(LineLayout &layout);
            
            void setSize(float size);
            void setMiddleLineFactor(float factor); // DEFAULT-VALUE IS 0, OTHERWISE getOffsetY() FOR "ALIGN_MIDDLE" WILL RETURN middleLineFactor * (getAscent() - getDescent())
            void setColor(const ci::ColorA &color);
            void setColor(float r, float g, float b, float a);
            
            void setClip(const ci::Rectf &clipRect);
            void setClip(float x1, float y1, float x2, float y2);
            void clearClip(); // INVOKED UPON SEQUENCE-BEGIN
            
            inline QuadMatrix& getMatrix()
            {
                return matrix;
            }
            
            template <typename T>
            inline QuadMatrix& loadMatrix(const T &other)
            {
                return matrix.load(other);
            }
            
            inline const uint16_t* getIndices() const
            {
                return const_cast<uint16_t*>(indices.data());
            }
            
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

            static float snap(float value);

            static Style stringToStyle(const std::string &style);
            static std::string styleToString(Style style);
            
            static hb_direction_t stringToDirection(const std::string &direction);
            static std::string directionToString(hb_direction_t direction);
            
        protected:
            friend class FontManager;

            float size;
            float sizeRatio;
            float middleLineFactor = 0;
            ci::ColorA color;
            
            bool hasClip = false;
            ci::Rectf clipRect;

            Properties properties;
            const std::vector<uint16_t> &indices;
            QuadMatrix matrix;
            
            float anisotropy;
            
            int began = 0;
            bool sequenceUseColor;
            FontSequence *sequence = nullptr;
            std::unique_ptr<QuadBatchMap<FontTexture>> batchMap;
            
            std::map<hb_language_t, FontSet> fontSetMap;
            
            VirtualFont(FontManager &fontManager, const Properties &properties);
            
            bool addActualFont(const std::string &lang, ActualFont *font);
            const FontSet& getFontSet(hb_language_t lang) const;
            
            void begin(bool useColor = false);
            void end(bool useColor = false);
            
            void incrementSequence(QuadBatch *batch);
            bool clipQuad(Quad &quad, FontTexture *texture) const;
        };
    }
}
