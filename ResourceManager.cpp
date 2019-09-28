#include "ResourceManager.h"
#include <assert.h>
#include <chrono>
#include <mutex>
#include "Renderer.h"
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

ResourceManager::~ResourceManager()
{
    is_alive = false;
    m_monitor_thread.join();
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

ProceduralTextureId ResourceManager::make_procedural_texture(const std::vector<Id>& textures)
{
    sf::Image image;
    image.create(g_texture_size, g_texture_size);
    for (const Id& texture_id : textures)
    {
        if (!texture_id.invalid_id())
            image.copy((*texture_id.texture()).copyToImage(), 0, 0, sf::IntRect(0,0,0,0), true);
    }
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

VertexShaderId ResourceManager::get_vertex_shader_id(const std::string& identifier)
{
    if (m_vertex_shader_ids.find(identifier) == m_vertex_shader_ids.end())
    {
        return InvalidId;
    }
    return m_vertex_shader_ids[identifier];
}

sf::Shader* ResourceManager::get_vertex_shader(VertexShaderId id)
{
    assert(id != InvalidId);
    return m_vertex_shaders[id].get();
}

FragmentShaderId ResourceManager::get_fragment_shader_id(const std::string& identifier)
{
    if (m_fragment_shader_ids.find(identifier) == m_fragment_shader_ids.end())
    {
        return InvalidId;
    }
    return m_fragment_shader_ids[identifier];
}

sf::Shader* ResourceManager::get_fragment_shader(FragmentShaderId id)
{
    assert(id != InvalidId);
    return m_fragment_shaders[id].get();
}

FullShaderId ResourceManager::get_full_shader_id(const std::string& identifier)
{
    if (m_full_shader_ids.find(identifier) == m_full_shader_ids.end())
    {
        return InvalidId;
    }
    return m_full_shader_ids[identifier];
}

sf::Shader* ResourceManager::get_full_shader(FullShaderId id)
{
    assert(id != InvalidId);
    return m_full_shaders[id].get();
}

void ResourceManager::load_all()
{
#ifdef RES_MAN_DEBUG_ENTRIES
    log("Directory entries in m_res_root:");
    for (const auto& dir : stdfs::recursive_directory_iterator(m_res_root))
    {
        log(dir);
    }
#endif
    
    load_textures();
    load_vertex_shaders();
    load_fragment_shaders();
    load_full_shaders();
    
    m_monitor_thread = std::thread(ResourceManager::monitor_changes, this);
}

void ResourceManager::load_textures()
{
    log("Loading textures...");
    for (const auto& tex_file_path : stdfs::directory_iterator(m_res_root / "textures"))
    {
        log(" -> loading " << tex_file_path.path().stem() << "...");
        sf::Texture tx{};
        bool success = tx.loadFromFile(tex_file_path.path().string());
        if (!success)
            throw std::runtime_error("Texture " + tex_file_path.path().string() + " could not be loaded.");
        
        TextureId id = ++m_last_texture_id;
        m_texture_ids.emplace(tex_file_path.path().stem(), id);
        m_textures.emplace(id, tx);
    }
    log("Done.");
}

void ResourceManager::load_vertex_shaders()
{
    log("Loading vertex shaders...");
    for (const auto& vertex_file_path : stdfs::directory_iterator(m_res_root / "shaders" / "vertex"))
    {
        log(" -> loading " << vertex_file_path.path().stem() << "...");
        std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
        bool success = sh->loadFromFile(vertex_file_path.path().native(), sf::Shader::Type::Vertex);
        if (!success)
            throw std::runtime_error("Vertex shader " + vertex_file_path.path().string() + " could not be loaded.");
        
        VertexShaderId id = ++m_last_vertex_shader_id;
        m_vertex_shader_ids.emplace(vertex_file_path.path().stem(), id);
        m_vertex_shaders.emplace(id, sh);
    }
    log("Done.");
}

void ResourceManager::load_fragment_shaders()
{
    log("Loading fragment shaders...");
    for (const auto& fragment_file_path : stdfs::directory_iterator(m_res_root / "shaders" / "fragment"))
    {
        log(" -> loading " << fragment_file_path.path().stem() << "...");
        std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
        bool success = sh->loadFromFile(fragment_file_path.path().native(), sf::Shader::Type::Fragment);
        if (!success)
            throw std::runtime_error("Fragment shader " + fragment_file_path.path().stem().string() + " could not be loaded.");
        sh->setUniform("texture", sf::Shader::CurrentTexture);
        FragmentShaderId id = ++m_last_fragment_shader_id;
        m_fragment_shader_ids.emplace(fragment_file_path.path().stem(), id);
        m_fragment_shaders.emplace(id, sh);
    }
    log("Done.");
}

void ResourceManager::load_full_shaders()
{
    log("Loading full shaders...");
    for (const auto& vert_file : stdfs::directory_iterator(m_res_root / "shaders" / "vertex"))
    {
        // If file with the same stem and .frag extension exists we can compile a full shader.
        stdfs::path frag_file = m_res_root / "shaders" / "fragment" / (vert_file.path().stem().string() + ".frag");
        if (stdfs::exists(frag_file))
        {
            std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
            bool success = sh->loadFromFile(vert_file.path().string(), frag_file.string());
            if (!success)
                throw std::runtime_error("Full shader " + vert_file.path().stem().string() + " could not be loaded.");
            
            sh->setUniform("texture", sf::Shader::CurrentTexture);
            FullShaderId id = ++m_last_full_shader_id;
            m_full_shader_ids.emplace(vert_file.path().stem(), id);
            m_full_shaders.emplace(id, sh);
        }
    }
    log("Done.");
}

// FIXME: Error code return type?
// Threaded
void ResourceManager::monitor_changes(ResourceManager* resman)
{
    std::map<std::string, stdfs::file_time_type> textures;
    for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "textures"))
        textures[file.path().string()] = file.last_write_time();
    
    std::map<std::string, stdfs::file_time_type> vertex_shaders;
    for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "shaders" / "vertex"))
        vertex_shaders[file.path().string()] = file.last_write_time();
    
    std::map<std::string, stdfs::file_time_type> fragment_shaders;
    for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "shaders" / "fragment"))
        fragment_shaders[file.path().string()] = file.last_write_time();
    
    while(resman->is_alive)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // FIXME: Choose a sane interval.
        
        for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "textures"))
        {
            stdfs::file_time_type now = file.last_write_time();
            const stdfs::path& path = file.path();
            if (now != textures[path.native()])
            {
                // FIXME: Fix this shit.
                log("Reloading texture " << path.stem().native() << "...");
                while (!g_renderer->render_mx.try_lock());
                bool success = 
                        resman->m_textures[resman->m_texture_ids[path.stem().native()]].loadFromFile(path.native());
                g_renderer->render_mx.unlock();
                if (!success)
                    throw std::runtime_error("Texture " + path.stem().string() + " could not be reloaded.");
            }
            textures[file.path().native()] = now;
        }
        
        for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "shaders" / "vertex"))
        {
            stdfs::file_time_type now = file.last_write_time();
            const stdfs::path& path = file.path();
            if (now != vertex_shaders[path.native()])
            {
                log("Reloading vertex shader " << path.stem().native() << "...");
                while (!g_renderer->render_mx.try_lock());
                std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
                bool success = sh->loadFromFile(path.native(), sf::Shader::Type::Vertex);
                if (!success)
                {
                    log("Reloading failed.");
                }
                else
                {
                    resman->m_vertex_shaders[resman->m_vertex_shader_ids[path.stem().native()]] = sh;
                }
                
                stdfs::path frag_file = resman->m_res_root / "shaders" / "fragment" / (path.stem().string() + ".frag");
                if (stdfs::exists(frag_file))
                {
                    std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
                    bool success = sh->loadFromFile(path.string(), frag_file.string());
                    if (!success)
                    {
                        log("Reloading full shader failed.");
                    }
                    else
                    {
                        resman->m_full_shaders[resman->m_full_shader_ids[path.stem().native()]] = sh;
                    }
                }
                
                g_renderer->render_mx.unlock();
            }
            vertex_shaders[file.path().native()] = now;
        }
        
        for (const auto& file : stdfs::directory_iterator(resman->m_res_root / "shaders" / "fragment"))
        {
            stdfs::file_time_type now = file.last_write_time();
            const stdfs::path& path = file.path();
            if (now != fragment_shaders[path.native()])
            {
                log("Reloading fragment shader " << path.stem().native() << "...");
                while (!g_renderer->render_mx.try_lock());
                std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
                bool success = sh->loadFromFile(path.native(), sf::Shader::Type::Fragment);
                if (!success) 
                {
                    log("Reloading failed.");
                }
                else
                {
                    resman->m_fragment_shaders[resman->m_fragment_shader_ids[path.stem().native()]] = sh;
                }
                
                stdfs::path other_file = resman->m_res_root / "shaders" / "vertex" / (path.stem().string() + ".vert");
                if (stdfs::exists(other_file))
                {
                    std::shared_ptr<sf::Shader> sh = std::make_shared<sf::Shader>();
                    bool success = sh->loadFromFile(other_file.string(), path.string());
                    if (!success)
                    {
                        log("Reloading full shader failed.");
                    }
                    else
                    {
                        resman->m_full_shaders[resman->m_full_shader_ids[path.stem().native()]] = sh;
                    }
                }
                
                g_renderer->render_mx.unlock();
            }
            fragment_shaders[file.path().native()] = now;
        }
    }
    log("ResourceManager::monitor_changes reached end.");
}
