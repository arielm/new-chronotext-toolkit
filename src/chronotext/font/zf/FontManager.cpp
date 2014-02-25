/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/FontManager.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace zf
    {
        enum
        {
            PLATFORM_OSX,
            PLATFORM_WINDOW,
            PLATFORM_IOS,
            PLATFORM_ANDROID
        };
        
        const string PLATFORM_NAMES[4] = {"osx", "windows", "ios", "android"};
        
        FontManager::FontManager()
        :
        ftHelper(make_shared<FreetypeHelper>()),
        itemizer(langHelper),
        hasDefaultFont(false)
        {
#if defined(CINDER_MAC)
            platform = PLATFORM_OSX;
#elif defined(CINDER_MSW)
            platform = PLATFORM_WINDOW;
#elif defined(CINDER_COCOA_TOUCH)
            platform = PLATFORM_IOS;
#elif defined(CINDER_ANDROID)
            platform = PLATFORM_ANDROID;
#else
            assert(false);
#endif
        }
        
        void FontManager::loadConfig(InputSourceRef source)
        {
            if (!globalMap.empty() || !aliases.empty() || hasDefaultFont)
            {
                throw runtime_error("FONT-CONFIG ALREADY DEFINED");
            }
            else
            {
                XmlTree doc(source->loadDataSource()); // EARLY-THROW IF THE DOCUMENT IS MALFORMED
                
                /*
                 * THE FOLLOWING IS NOT SUPPOSED TO THROW...
                 * IN THE WORST-CASE: THE MAP WILL BE EMPTY OR PARTIAL
                 *
                 * TODO: THROW WHEN DOCUMENT IS INVALID?
                 */
                if (doc.hasChild("FontConfig"))
                {
                    if (doc.hasChild("FontConfig/DefaultFont"))
                    {
                        hasDefaultFont = true;
                        
                        auto &defaultFontElement = doc.getChild("FontConfig/DefaultFont");
                        defaultFontName = defaultFontElement.getAttributeValue<string>("name", "");
                        defaultFontStyle = VirtualFont::styleStringToEnum(defaultFontElement.getAttributeValue<string>("style", "regular"));
                    }
                    
                    for (auto aliasElement = doc.begin("FontConfig/Aliases/Alias"); aliasElement != doc.end(); ++aliasElement)
                    {
                        auto name = aliasElement->getAttributeValue<string>("name", "");
                        auto target = aliasElement->getAttributeValue<string>("target", "");
                        
                        if (!name.empty() && !target.empty())
                        {
                            aliases[name] = target;
                        }
                    }
                    
                    for (auto fontElement = doc.begin("FontConfig/VirtualFonts/VirtualFont"); fontElement != doc.end(); ++fontElement)
                    {
                        auto name = fontElement->getAttributeValue<string>("name", "");
                        
                        if (!name.empty())
                        {
                            auto style = VirtualFont::styleStringToEnum(fontElement->getAttributeValue<string>("style", "regular"));
                            auto baseSize = fontElement->getAttributeValue<float>("base-size", 0);
                            
                            for (auto &refElement : fontElement->getChildren())
                            {
                                auto os = refElement->getAttributeValue<string>("os", "");
                                
                                if (os == PLATFORM_NAMES[platform])
                                {
                                    auto uri = refElement->getAttributeValue<string>("uri", "");
                                    
                                    if (!uri.empty())
                                    {
                                        globalMap[make_pair(name, style)] = make_pair(uri, baseSize);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        shared_ptr<VirtualFont> FontManager::getCachedFont(const std::string &name, VirtualFont::Style style, const VirtualFont::Properties &properties)
        {
            auto key = make_tuple(name, style, properties);
            auto it1 = shortcuts.find(key);
            
            if (it1 != shortcuts.end())
            {
                return it1->second;
            }
            
            // ---
            
            string nameOrAlias;
            auto it2 = aliases.find(name);
            
            if (it2 == aliases.end())
            {
                nameOrAlias = name;
            }
            else
            {
                nameOrAlias = it2->second;
            }
            
            // ---
            
            auto it3 = globalMap.find(make_pair(nameOrAlias, style));
            
            if (it3 != globalMap.end())
            {
                float baseSize = properties.baseSize;
                
                /*
                 * TRYING TO USE THE base-size ATTRIBUTE AT THE FONT-CONFIG LEVEL
                 */
                if (baseSize == 0)
                {
                    baseSize = it3->second.second;
                }
                
                if (baseSize == 0)
                {
                    throw invalid_argument("UNDEFINED FONT-SIZE");
                }
                
                auto uri = it3->second.first;
                auto font = getCachedFont(InputSource::get(uri), VirtualFont::Properties(baseSize, properties.useMipmap, properties.useAnisotropy, properties.slotCapacity)); // CAN THROW
                
                /*
                 * ALLOWING CACHING UPON FURTHER ACCESS
                 */
                shortcuts[key] = font;
                return font;
            }
            
            /*
             * BASIC SYSTEM FOR HANDLING UNDEFINED FONT NAMES AND STYLES
             */
            if (hasDefaultFont)
            {
                if (name != defaultFontName)
                {
                    auto font = getCachedFont(defaultFontName, style, properties);
                    shortcuts[key] = font;
                    return font;
                }
                else if (style != defaultFontStyle)
                {
                    auto font = getCachedFont(defaultFontName, defaultFontStyle, properties);
                    shortcuts[key] = font;
                    return font;
                }
            }
            
            /*
             * SHOULD NOT OCCUR, UNLESS NO "DEFAULT FONT" IS DEFINED
             */
            throw invalid_argument(string("UNDEFINED FONT: ") + name + " " + VirtualFont::styleEnumToString(style));
        }
        
        shared_ptr<VirtualFont> FontManager::getCachedFont(InputSourceRef source, const VirtualFont::Properties &properties)
        {
            auto key = make_pair(source->getURI(), properties);
            auto it = virtualFonts.find(key);
            
            if (it != virtualFonts.end())
            {
                return it->second;
            }
            else
            {
                XmlTree doc(source->loadDataSource()); // EARLY-THROW IF THE DOCUMENT IS MALFORMED
                
                /*
                 * THE FOLLOWING IS NOT SUPPOSED TO THROW...
                 * IN THE WORST-CASE: THE FONT WILL BE "EMPTY" OR "PARTIAL"
                 *
                 * TODO: THROW WHEN DOCUMENT IS INVALID?
                 */
                if (doc.hasChild("VirtualFont"))
                {
                    auto font = shared_ptr<VirtualFont>(new VirtualFont(*this, properties)); // make_shared WON'T WORK WITH A PROTECTED CONSTRUCTOR
                    virtualFonts[key] = font;
                    
                    for (auto setElement = doc.begin("VirtualFont/Set"); setElement != doc.end(); ++setElement)
                    {
                        auto langList = splitLanguages(setElement->getAttributeValue<string>("lang", ""));
                        
                        for (auto &lang : langList)
                        {
                            for (auto &variantElement : setElement->getChildren())
                            {
                                if (variantElement->getTag() == "Group")
                                {
                                    for (auto &refElement : variantElement->getChildren())
                                    {
                                        auto descriptor = parseDescriptor(*refElement);
                                        
                                        if (!descriptor.empty() && font->addActualFont(lang, getActualFont(descriptor, properties.baseSize, properties.useMipmap)))
                                        {
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    auto descriptor = parseDescriptor(*variantElement);
                                    
                                    if (!descriptor.empty())
                                    {
                                        font->addActualFont(lang, getActualFont(descriptor, properties.baseSize, properties.useMipmap));
                                    }
                                }
                            }
                        }
                    }
                    
                    return font;
                }
                
                throw invalid_argument("INVALID FONT-DEFINITION: " + source->getURI());
            }
        }
        
        void FontManager::unload(shared_ptr<VirtualFont> virtualFont)
        {
            for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it)
            {
                if (it->second == virtualFont)
                {
                    shortcuts.erase(it);
                    break;
                }
            }
            
            for (auto it = virtualFonts.begin(); it != virtualFonts.end(); ++it)
            {
                if (it->second == virtualFont)
                {
                    virtualFonts.erase(it);
                    break;
                }
            }
            
            // ---
            
            set<ActualFont*> actualFontsInUse;
            
            for (auto &it1 : virtualFonts)
            {
                for (auto &it2 : it1.second->fontSetMap)
                {
                    for (auto &it3 : it2.second)
                    {
                        actualFontsInUse.insert(it3);
                    }
                }
            }
            
            for (auto it = actualFonts.begin(); it != actualFonts.end(); ++it)
            {
                if (!actualFontsInUse.count(it->second.get()))
                {
                    it->second->unload();
                }
            }
        }
        
        void FontManager::unload()
        {
            for (auto &it : actualFonts)
            {
                it.second->unload();
            }
        }
        
        void FontManager::discardTextures()
        {
            for (auto &it : actualFonts)
            {
                it.second->discardTextures();
            }
        }
        
        void FontManager::reloadTextures()
        {
            for (auto &it : actualFonts)
            {
                it.second->reloadTextures();
            }
        }
        
        size_t FontManager::getTextureMemoryUsage() const
        {
            size_t total = 0;
            
            for (auto &it : actualFonts)
            {
                total += it.second->getTextureMemoryUsage();
            }
            
            return total;
        }
        
        ActualFont* FontManager::getActualFont(const ActualFont::Descriptor &descriptor, float baseSize, bool useMipmap)
        {
            ActualFont::Key key(descriptor, baseSize, useMipmap);
            auto it = actualFonts.find(key);
            
            if (it != actualFonts.end())
            {
                return it->second.get();
            }
            else
            {
                try
                {
                    auto font = new ActualFont(ftHelper, descriptor, baseSize, useMipmap);
                    actualFonts[key] = unique_ptr<ActualFont>(font);
                    
                    return font;
                }
                catch (exception &e)
                {
                    LOGD << e.what() << " - " << descriptor.source->getURI() << endl;
                }
                
                return NULL;
            }
        }
        
        vector<string> FontManager::splitLanguages(const string &languages)
        {
            return split(languages, ":");
        }
        
        ActualFont::Descriptor FontManager::parseDescriptor(const XmlTree &element)
        {
            auto uri = element.getAttributeValue<string>("uri", "");
            auto faceIndex = element.getAttributeValue<int>("face-index", 0);
            auto scale = element.getAttributeValue<float>("scale", 1);
            
            if (!uri.empty())
            {
                return ActualFont::Descriptor(InputSource::get(uri), faceIndex, scale);
            }
            else
            {
                return ActualFont::Descriptor();
            }
        }
        
        const vector<GLushort>& FontManager::getIndices(int capacity)
        {
            if (capacity * 6 > indices.size())
            {
                /*
                 * FILLING THE INDICES WITH A QUAD PATTERN:
                 * - FROM UPPER-LEFT CORNER
                 * - COUNTER-CLOCKWISE
                 */
                
                indices.clear();
                indices.reserve(capacity * 6);
                
                int offset = 0;
                
                for (int i = 0; i < capacity; i++)
                {
                    indices.push_back(offset);
                    indices.push_back(offset + 1);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 3);
                    indices.push_back(offset);
                    offset += 4;
                }
            }
            
            return indices;
        }
    }
}
