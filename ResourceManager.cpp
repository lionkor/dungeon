#include "ResourceManager.h"
#include <assert.h>
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

sf::Texture* ResourceManager::get_texture(const std::string& identifier)
{
    return &m_textures[identifier];
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
        m_textures.emplace(tex_file_path.path().stem(), tx);
    }
}
