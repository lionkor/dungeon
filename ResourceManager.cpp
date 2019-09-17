#include "ResourceManager.h"
#include <assert.h>
#include <mutex>
namespace stdfs=std::filesystem;



ResourceManager::ResourceManager(stdfs::path resource_root_directory)
    : m_res_root(stdfs::absolute(resource_root_directory)),
      m_textures()
{
    assert(stdfs::exists(m_res_root)); // FIXME: Do we need this?
    // Loading is a function so we can call it from, for example, 
    //  a file change monitor for hot-reloading.
    load_all();
}

TextureId ResourceManager::get_texture_id(const std::string& identifier)
{
    if (m_texture_ids.find(identifier) == m_texture_ids.end())
    {
        return InvalidId;
    }
    return m_texture_ids[identifier];
}

sf::Texture* ResourceManager::get_texture(TextureId texture_id)
{
    assert(texture_id != InvalidId); // FIXME: Don't include this assert in release.
    return &m_textures[texture_id];
}

void ResourceManager::load_all()
{
    log("Directory entries in m_res_root:");
    for (const auto& dir : stdfs::recursive_directory_iterator(m_res_root))
    {
        log(dir);
    }
    
    load_textures();
}

void ResourceManager::load_textures()
{
    log("Loading textures...");
    for (const auto& tex_file_path : stdfs::directory_iterator(m_res_root / "textures"))
    {
        log(" -> loading " << tex_file_path.path().stem() << "...");
        sf::Texture tx{};
        tx.loadFromFile(tex_file_path.path().native());
        // FIXME: Check return value, handle failure.
        
        // Just in case we ever want to multithread this,
        //  so I don't forget.
        std::mutex mx;
        mx.lock();
        TextureId id = ++m_last_id;
        m_texture_ids.emplace(tex_file_path.path().stem(), id);
        m_textures.emplace(id, tx);
        mx.unlock();
    }
}
