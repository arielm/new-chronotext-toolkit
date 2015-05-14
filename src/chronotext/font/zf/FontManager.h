/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"
#include "chronotext/font/zf/LayoutStore.h"
#include "chronotext/font/zf/TextItemizer.h"
#include "chronotext/InputSource.h"

#include "cinder/Xml.h"

#include <atomic>

namespace chr
{
    namespace zf
    {
        class FontManager
        {
        public:
            static std::atomic<bool> LOG_VERBOSE;
            static std::atomic<bool> LOG_WARNING;
            static std::atomic<bool> PROBE_MEMORY;
            
            // ---

            std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES
            std::shared_ptr<LangHelper> langHelper;
            std::shared_ptr<LayoutStore> layoutStore;
            std::shared_ptr<TextItemizer> itemizer;
            
            FontManager();
            
            /*
             * THE FONT-CONFIG, MAPPING BETWEEN:
             * - A STRING LIKE "sans-serif" AND SOME STYLE (E.G. VirtualFont::STYLE_BOLD)
             * - AND SOME FONT XML-DEFINITION LIKE "sans-serif_bold_ios.xml"
             *
             * NOT MANDATORY, BUT SHOULD BE DEFINED ONLY ONCE
             */
            void loadConfig(InputSource::Ref source);
            
            /*
             * HIGHER-LEVEL METHOD, REQUIRING A FONT-CONFIG
             *
             * SCENARIO 1:
             * - CALLER IS DEFINING A FONT-SIZE (baseSize != 0)
             * - IF A FONT-SIZE IS DEFINED AT THE FONT-CONFIG LEVEL (base-size ATTRIBUTE), IT IS IGNORED
             * - MIPMAPS ARE NOT ALLOWED
             * - VirtualFont::setSize() IS NOT SUPPOSED TO BE USED DURING THE FONT'S LIFE-CYCLE
             *
             * SCENARIO 2:
             * - CALLER IS NOT DEFINING A FONT-SIZE (baseSize == 0)
             * - THE FONT-SIZE DEFINED AT THE FONT-CONFIG LEVEL (base-size ATTRIBUTE) IS USED
             * - MIPMAPS ARE ALLOWED
             * - VirtualFont::setSize() SHALL BE USED DURING THE FONT'S LIFE-CYCLE
             */
            std::shared_ptr<VirtualFont> getFont(const std::string &name, VirtualFont::Style style = VirtualFont::STYLE_REGULAR, const VirtualFont::Properties &properties = VirtualFont::Properties2d());
            
            /*
             * LOWER-LEVEL METHOD, FOR ACCESSING A FONT DIRECTLY VIA ITS XML-DEFINITION
             */
            std::shared_ptr<VirtualFont> getFont(InputSource::Ref source, const VirtualFont::Properties &properties);
            
            /*
             * CLEARS THE FONT RESOURCES (HARFBUZZ AND FREETYPE RELATED) AND DISCARDS THE GLYPH TEXTURES
             * ASSOCIATED SOLELY WITH A SPECIFIC VirtualFont
             * FROM THIS POINT: RESOURCES WILL BE RELOADED AND TEXTURES RECREATED ONLY WHEN NECESSARY
             */
            void unload(std::shared_ptr<VirtualFont> virtualFont);
            
            /*
             * CLEARS ALL THE FONT RESOURCES (HARFBUZZ AND FREETYPE RELATED) AND DISCARDS ALL THE GLYPH TEXTURES
             * FROM THIS POINT: RESOURCES WILL BE RELOADED AND TEXTURES RECREATED ONLY WHEN NECESSARY
             */
            void unload();
            
            /*
             * DISCARDS ALL THE GLYPH TEXTURES (MUST BE CALLED AFTER OPENGL CONTEXT-LOSS)
             * FROM THIS POINT: TEXTURES WILL BE RECREATED ONLY WHEN NECESSARY
             */
            void discardTextures();
            
            /*
             * FORCES ALL THE PREVIOUSLY DISCARDED GLYPH TEXTURES TO BE RELOADED
             */
            void reloadTextures();
            
            /*
             * RETURNS THE MEMORY USED BY ALL THE GLYPH TEXTURES
             * CURRENTLY: "ALPHA" TEXTURES ARE USED (ONE BYTE PER PIXEL)
             */
            int64_t getTextureMemoryUsage() const;
            
        protected:
            friend class VirtualFont;

            bool hasDefaultFont = false;
            std::string defaultFontName;
            VirtualFont::Style defaultFontStyle;
            
            std::map<std::pair<std::string, VirtualFont::Style>, std::pair<std::string, float>> globalMap;
            std::map<std::tuple<std::string, VirtualFont::Style, VirtualFont::Properties>, std::shared_ptr<VirtualFont>> shortcuts;
            std::map<std::string, std::string> aliases;

            std::vector<uint16_t> indices;
            
            std::map<std::pair<std::string, VirtualFont::Properties>, std::shared_ptr<VirtualFont>> virtualFonts;
            std::map<ActualFont::Key, std::unique_ptr<ActualFont>> actualFonts;
            
            ActualFont* getActualFont(const ActualFont::Descriptor &descriptor, float baseSize, bool useMipmap = false);
            
            static std::vector<std::string> splitLanguages(const std::string &languages);
            static ActualFont::Descriptor parseDescriptor(const ci::XmlTree &element);
            
            const std::vector<uint16_t>& getIndices(int capacity);
        };
    }
}
