/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"
#include "chronotext/InputSource.h"

#include "cinder/Xml.h"

namespace chronotext
{
    namespace zf
    {
        class FontManager
        {
        public:
            std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES
            LangHelper langHelper;
            LayoutCache layoutCache;
            TextItemizer itemizer;
            
            FontManager();
            
            /*
             * THE FONT-CONFIG, MAPPING BETWEEN:
             * - A STRING LIKE "sans-serif" AND SOME STYLE (E.G. VirtualFont::STYLE_BOLD)
             * - AND SOME FONT XML-DEFINITION LIKE "sans-serif_bold_ios.xml"
             *
             * NOT MANDATORY, BUT SHOULD BE DEFINED ONLY ONCE
             */
            void loadConfig(InputSourceRef source);
            
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
            std::shared_ptr<VirtualFont> getCachedFont(const std::string &name, VirtualFont::Style style = VirtualFont::STYLE_REGULAR, const VirtualFont::Properties &properties = VirtualFont::Properties2d());
            
            /*
             * LOWER-LEVEL METHOD, FOR ACCESSING A FONT DIRECTLY VIA ITS XML-DEFINITION
             */
            std::shared_ptr<VirtualFont> getCachedFont(InputSourceRef source, const VirtualFont::Properties &properties);
            
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
             * RETURNS THE MEMORY USED BY ALL THE GLYPH TEXTURES
             * CURRENTLY: "ALPHA" TEXTURES ARE USED (ONE BYTE PER PIXEL)
             * NOTE THAT THE GPU MAY DECIDE TO USE MORE MEMORY INTERNALLY
             */
            size_t getTextureMemoryUsage() const;
            
            friend class VirtualFont;
            
        protected:
            int platform;
            
            bool hasDefaultFont;
            std::string defaultFontName;
            VirtualFont::Style defaultFontStyle;
            
            std::map<std::pair<std::string, VirtualFont::Style>, std::pair<std::string, float>> globalMap;
            std::map<std::tuple<std::string, VirtualFont::Style, VirtualFont::Properties>, std::shared_ptr<VirtualFont>> shortcuts;
            std::map<std::string, std::string> aliases;

            std::vector<GLushort> indices;
            
            std::map<VirtualFont::Key, std::shared_ptr<VirtualFont>> virtualFonts;
            std::map<ActualFont::Key, std::unique_ptr<ActualFont>> actualFonts;
            
            ActualFont* getActualFont(const ActualFont::Descriptor &descriptor, float baseSize, bool useMipmap = false);
            
            static std::vector<std::string> splitLanguages(const std::string &languages);
            static ActualFont::Descriptor parseDescriptor(const ci::XmlTree &element);
            
            const std::vector<GLushort>& getIndices(int capacity);
        };
    }
}

namespace chr = chronotext;
