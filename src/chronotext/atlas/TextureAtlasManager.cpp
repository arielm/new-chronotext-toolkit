/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/atlas/TextureAtlasManager.h"
#include "chronotext/texture/TextureHelper.h"

using namespace ci;
using namespace std;

TextureAtlasManager::~TextureAtlasManager()
{
    for (map<int, TextureAtlas*>::iterator it = atlases.begin(); it != atlases.end(); ++it)
    {
        delete it->second;
    }
}

void TextureAtlasManager::setAtlas(const string &resourceName, int index, bool useMipmap)
{
    atlases[index] = new TextureAtlas(resourceName, useMipmap);
}

void TextureAtlasManager::setAtlas(chr::InputSourceRef inputSource, int index, bool useMipmap)
{
    atlases[index] = new TextureAtlas(inputSource, useMipmap);
}

TextureAtlas* TextureAtlasManager::getAtlas(int index)
{
    return atlases[index];
}

void TextureAtlasManager::beginTexture(int index)
{
    atlases[index]->beginTexture();
}

void TextureAtlasManager::endTexture()
{
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TextureAtlasManager::unload()
{
    if (!unloaded)
    {
        unloaded = true;
        
        for (map<int, TextureAtlas*>::iterator it = atlases.begin(); it != atlases.end(); ++it)
        {
            it->second->unload();
        }
    }
}

void TextureAtlasManager::reload()
{
    if (unloaded)
    {
        unloaded = false;
        
        for (map<int, TextureAtlas*>::iterator it = atlases.begin(); it != atlases.end(); ++it)
        {
            it->second->reload();
        }
    }
}
