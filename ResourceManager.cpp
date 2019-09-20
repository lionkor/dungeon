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
    assert(texture_id != InvalidId); // TODO: Don't include asserts in release.
    return &m_textures[texture_id];
}


ProceduralTextureId ResourceManager::make_procedural_texture(const std::vector<TextureId>& textures)
{
    sf::Image image;
    image.create(g_texture_size, g_texture_size);
    for (auto& texture_id : textures)
        image.copy(m_textures[texture_id].copyToImage(), 0, 0, sf::IntRect(0,0,0,0), true);
    sf::Texture texture;
    texture.loadFromImage(image);
    ProceduralTextureId id = ++m_last_procedural_texture_id;
    m_procedural_textures.emplace(id, texture);
    return id;
}

sf::Texture* ResourceManager::get_procedural_texture(ProceduralTextureId id)
{
    assert(id != InvalidId);
    return &m_procedural_textures[id];
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
        TextureId id = ++m_last_texture_id;
        m_texture_ids.emplace(tex_file_path.path().stem(), id);
        m_textures.emplace(id, tx);
        mx.unlock();
    }
}
