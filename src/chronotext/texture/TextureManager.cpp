#include "chronotext/texture/TextureManager.h"
#include "chronotext/utils/Utils.h"

#include "cinder/DataSource.h"
#include "cinder/app/App.h"

#include <sstream>

using namespace ci;
using namespace app;
using namespace std;
using namespace chr;

TextureManager::~TextureManager()
{
    for (map<uint64_t, gl::Texture*>::const_iterator it = cache.begin(); it != cache.end(); ++it)
    {
        delete it->second;
    }
    
    DLOG(cache.size() << " TEXTURES DELETED");
}

#if defined(CINDER_MSW)
gl::Texture* TextureManager::getTexture(int mswID, const string &mswType, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT)
{
    stringstream oss;
    oss << mswID << mswType << useMipmap << filter << wrapS << wrapT;
    
    string key = oss.str();
    uint64_t id = chr::hash(key);
    
    if (hasTexture(id))
    {
        return getTexture(id);
    }
    
    DataSourceRef resource = loadResource(mswID, mswType);
    
    gl::Texture *texture = TextureHelper::loadTexture(resource, useMipmap, filter, wrapS, wrapT);
    putTexture(id, texture);
    
    return texture;
}
#else
gl::Texture* TextureManager::getTexture(const string &macPath, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT)
{
    stringstream oss;
    oss << macPath << useMipmap << filter << wrapS << wrapT;
    
    string key = oss.str();
    uint64_t id = chr::hash(key);
    
    if (hasTexture(id))
    {
        return getTexture(id);
    }
    
    DataSourceRef resource = loadResource(macPath);
    
    gl::Texture *texture = TextureHelper::loadTexture(resource, useMipmap, filter, wrapS, wrapT);
    putTexture(id, texture);
    
    return texture;
}
#endif

bool TextureManager::removeTexture(gl::Texture *texture)
{
    for (map<uint64_t, gl::Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
    {
        if (texture == it->second)
        {
            delete it->second;
            cache.erase(it);

            return true;
        }
    }
    
    return false;
}
