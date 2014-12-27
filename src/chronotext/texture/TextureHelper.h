/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) PROBE (CPU) TIME AS WELL
 *
 * 2) FAIL EARLIER WHEN "MAX-SIZE" IS EXCEEDED:
 *    - CURRENTLY NECESSARY IN:
 *      - fetchTextureData() WHEN CALLING PVRHelper::decompressGZ() OR PVRHelper::decompressCCZ
 *        - TRIVIAL: THE INFO LIES IN THE "PVR HEADER"
 *      - fetchTranslucentTextureData() AND fetchPowerOfTwoTextureData()
 *        - NON TRIVIAL:
 *          - CINDER'S IMAGE-IO PROTOCOL DOES NOT SEEM TO BE ADAPTED:
 *            - I.E. THERE SHOULD BE SOME USER-DEFINABLE CALLBACK, INVOKED AS-SOON-AS THE IMAGE-HEADER IS PARSED
 *              - OBVIOUSLY: MOST OF THE IMAGE-LOADING SOLUTIONS (E.G. stb_image) ALLOW TO "GUESS" IMAGE-SIZE BEFORE FULLY LOADING IT
 */

#pragma once

#include "chronotext/texture/TextureData.h"
#include "chronotext/system/MemoryInfo.h"

#include <map>

namespace chr
{
    class TextureHelper
    {
    public:
        static bool PROBE_MEMORY;
        static bool RGB_USE_4_CHANNELS;

        struct MemoryProbe
        {
            Texture::Request textureRequest;
            size_t memoryUsage;
            MemoryInfo memoryInfo[3];
        };
        
        static MemoryInfo memoryInfo[2];
        static std::map<ci::gl::Texture*, MemoryProbe> probes;

        // ---
        
        static ci::gl::TextureRef loadTexture(InputSource::Ref inputSource, bool useMipmap = false, Texture::Request::Flags flags = Texture::Request::FLAGS_NONE);
        static ci::gl::TextureRef loadTexture(const Texture::Request &textureRequest);
        
        static TextureData fetchTextureData(const Texture::Request &textureRequest);
        static ci::gl::TextureRef uploadTextureData(const TextureData &textureData);
        
        static ci::Vec2i getTextureSize(const TextureData &textureData);
        static size_t getTextureMemoryUsage(const TextureData &textureData);
        
    protected:
        static void textureDeallocator(void *refcon);
        static bool isOverSized(const Texture::Request &textureRequest, const ci::Vec2i &size);

        static TextureData fetchTranslucentTextureData(const Texture::Request &textureRequest);
        static TextureData fetchPowerOfTwoTextureData(const Texture::Request &textureRequest);
    };
}
