/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/PVRHelper.h"
#include "chronotext/texture/Texture.h"

#include "cinder/Utilities.h"

#include <zlib.h>

using namespace std;
using namespace ci;
using namespace chr;

#define PVR_TEXTURE_FLAG_TYPE_MASK 0xff

/*
 * PVR VERSION 2
 */
struct PVRTexHeader
{
    uint32_t headerLength;
    uint32_t height;
    uint32_t width;
    uint32_t numMipmaps;
    uint32_t flags;
    uint32_t dataLength;
    uint32_t bpp;
    uint32_t bitmaskRed;
    uint32_t bitmaskGreen;
    uint32_t bitmaskBlue;
    uint32_t bitmaskAlpha;
    uint32_t pvrTag;
    uint32_t numSurfs;
};

enum
{
    kPVRTexturePixelTypeRGBA_4444 = 0x10,
    kPVRTexturePixelTypeRGBA_5551,
    kPVRTexturePixelTypeRGBA_8888,
    kPVRTexturePixelTypeRGB_565,
    kPVRTexturePixelTypeRGB_555,
    kPVRTexturePixelTypeRGB_888,
    kPVRTexturePixelTypeI_8,
    kPVRTexturePixelTypeAI_88,
    kPVRTexturePixelTypePVRTC_2,
    kPVRTexturePixelTypePVRTC_4,    
    kPVRTexturePixelTypeBGRA_8888,
    kPVRTexturePixelTypeA_8
};

struct CCZHeader
{
    uint8_t sig[4];            // signature: should be 'CCZ!'
    uint16_t compression_type; // should be CCZ_COMPRESSION_ZLIB
    uint16_t version;          // should be 2 (although 1 is also supported)
    uint32_t reserved;         // reserved for users
    uint32_t len;              // size of the uncompressed file
};

enum
{
    CCZ_COMPRESSION_ZLIB,  // zlib format.
    CCZ_COMPRESSION_BZIP2, // bzip2 format (not supported yet)
    CCZ_COMPRESSION_GZIP,  // gzip format (not supported yet)
    CCZ_COMPRESSION_NONE,  // plain (not supported yet)
};

static bool isPOT(int n)
{
    return (n && !(n & (n - 1)));
}

/*
 * BASED ON http://www.codeandweb.com/blog/2011/05/03/loading-gzip-compressed-pvr-textures-without-realloc
 */
Buffer PVRHelper::decompressPVRGZ(const fs::path &filePath)
{
    gzFile file = gzopen(filePath.string().data(), "rb");
    if (!file)
    {
        throw chr::Exception<Texture>("PVR.GZ: CAN'T OPEN FILE");
    }
    
    PVRTexHeader header;
    if (gzread(file, &header, sizeof(header)) != sizeof(header))
    {
        gzclose(file);
        throw chr::Exception<Texture>("PVR.GZ: HEADER ERROR");
    }
    
    if (header.pvrTag != 559044176)
    {
        gzclose(file);
        throw chr::Exception<Texture>("PVR.GZ: FORMAT ERROR");
    }
    
    Buffer buffer(sizeof(header) + header.dataLength);
    
    char *data = (char*)buffer.getData();
    if (!data)
    {
        gzclose(file);
        throw chr::Exception<Texture>("PVR.GZ: OUT-OF-MEMORY");
    }
    
    memcpy(data, &header, sizeof(header));
    
    if (gzread(file, data + sizeof(header), header.dataLength) != header.dataLength)
    {
        gzclose(file);
        throw chr::Exception<Texture>("PVR.GZ: DECOMPRESSION ERROR");
    }
    
    gzclose(file);
    return buffer;
}

/*
 * BASED ON https://github.com/cocos2d/cocos2d-iphone/blob/develop/cocos2d/Support/ZipUtils.m
 */
Buffer PVRHelper::decompressPVRCCZ(DataSourceRef dataSource)
{
    Buffer tmp = dataSource->getBuffer();
    
    CCZHeader *header = (CCZHeader*)tmp.getData();
    if ((header->sig[0] != 'C') || (header->sig[1] != 'C') || (header->sig[2] != 'Z') || (header->sig[3] != '!'))
    {
        throw chr::Exception<Texture>("PVR.CCZ: FORMAT ERROR");
    }

    /*
     * ASSERTION: THE SYSTEM IS LITTLE-ENDIAN (OSX, WINDOWS, iOS, ANDROID)
     */
    uint16_t compression_type = swapEndian(header->compression_type);
    uint16_t version = swapEndian(header->version);
    uint32_t len = swapEndian(header->len);
    
    if (compression_type != CCZ_COMPRESSION_ZLIB)
    {
        throw chr::Exception<Texture>("PVR.CCZ: UNSUPPORTED COMPRESSION FORMAT");
    }

    if (version > 2)
    {
        throw chr::Exception<Texture>("PVR.CCZ: UNSUPPORTED VERSION");
    }

    Buffer buffer(len);
    
    void *out = buffer.getData();
    if (!out)
    {
        throw chr::Exception<Texture>("PVR.CCZ: OUT-OF-MEMORY");
    }
    
    uLongf destlen = len;
    uLongf source = (uLongf)tmp.getData() + sizeof(*header);
    int ret = uncompress((Bytef*)out, &destlen, (Bytef*)source, tmp.getDataSize() - sizeof(*header));
    
    if (ret != Z_OK)
    {
        throw chr::Exception<Texture>("PVR.CCZ: DECOMPRESSION ERROR");
    }
    
    return buffer;
}

gl::TextureRef PVRHelper::getPVRTexture(const Buffer &buffer, bool useMipmap, GLenum wrapS, GLenum wrapT)
{
    PVRTexHeader *header = (PVRTexHeader*)buffer.getData();
    
    uint32_t width = header->width;
    uint32_t height = header->height;
    
    if (!isPOT(width) || !isPOT(height))
    {
        throw chr::Exception<Texture>("PVR TEXTURE: DIMENSIONS MUST BE A POWER-OF-TWO");
    }

    GLenum internalFormat;
    GLenum format;
    GLenum type;
    
    switch (header->flags & PVR_TEXTURE_FLAG_TYPE_MASK)
    {
        case kPVRTexturePixelTypeRGBA_4444:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            type = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
            
        case kPVRTexturePixelTypeRGBA_8888:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
            
        case kPVRTexturePixelTypeRGB_565:
            internalFormat = GL_RGB;
            format = GL_RGB;
            type = GL_UNSIGNED_SHORT_5_6_5;
            break;
            
        case kPVRTexturePixelTypeRGB_888:
            internalFormat = GL_RGB;
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
            
        case kPVRTexturePixelTypeA_8:
            internalFormat = GL_ALPHA;
            format = GL_ALPHA;
            type = GL_UNSIGNED_BYTE;
            break;
            
        default:
            throw chr::Exception<Texture>("PVR TEXTURE: UNSUPPORTED PIXEL-TYPE");
    }
    
    char *data = (char*)buffer.getData() + header->headerLength;
    
    uint32_t name;
    glGenTextures(1, &name);
    
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    
    if (useMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    
    if (useMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    }
    
    return gl::Texture::create(GL_TEXTURE_2D, name, width, height, false);
}

Vec2i PVRHelper::getPVRTextureSize(const Buffer &buffer)
{
    PVRTexHeader *header = (PVRTexHeader*)buffer.getData();
    return Vec2i(header->width, header->height);
}
