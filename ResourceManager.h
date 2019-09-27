#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Global.h"
#include <filesystem>
#include <assert.h>
#include <thread>

/*
 * ID System
 * 
 * Concept:
 *  Each Resource has a container that maps the name to an id, and a container
 *   that maps the id to the Resource. With this a comprehensive lookup can
 *   be done once (with the name as the query), and then a dynamic id is 
 *   generated for the current session which corresponds to a texture in
 *   the id-resource map.
 *  
 *  The idea behind this is that the string names of the textures can be 
 *   hardcoded, while having the actual resource retrieval be idependent
 *   of a lookup based on strings, and instead use a lookup of an unsigned
 *   integer.
 * 
 *  This keeps the ResourceManager in control over all resources and does
 *   not allow any other object to store a long term pointer or reference.
 *   If a resource must be reloaded, it can be removed and added under the
 *   same id without issues.
 * 
 * Usage:
 *  Each object that owns a resource retrieves an id using the corresponding
 *   get_resource_id function. This id can then get, for example, be passed
 *   to the Renderer by id, who will then perform a cheap lookup.
 */

inline static const Id_t InvalidId = 0;

enum class IdTextureType
{
    Invalid,
    TextureId,
    ProceduralTextureId
};

class ResourceManager
{
public:
    ResourceManager(std::filesystem::path);
    ~ResourceManager();
    
    TextureId get_texture_id(const std::string&);
    sf::Texture* get_texture(TextureId);
    
    // Caution: This function is very slow.
    ProceduralTextureId make_procedural_texture(const std::vector<TextureId>&);
    sf::Texture* get_procedural_texture(ProceduralTextureId);
    
    VertexShaderId get_vertex_shader_id(const std::string&);
    sf::Shader* get_vertex_shader(VertexShaderId);
    
    FragmentShaderId get_fragment_shader_id(const std::string&);
    sf::Shader* get_fragment_shader(FragmentShaderId);
    
    FullShaderId get_full_shader_id(const std::string&);
    sf::Shader* get_full_shader(FullShaderId);
    
    void load_all();
private:
    void load_textures();
    void load_vertex_shaders();
    void load_fragment_shaders();
    void load_full_shaders();
    
    // Monitors changes to the res root filesystem and hot-reloads resources.
    static void monitor_changes(ResourceManager*);
    
    std::thread m_monitor_thread;
    bool is_alive { true };
    
    TextureId m_last_texture_id { 1 };
    ProceduralTextureId m_last_procedural_texture_id { 1 };
    VertexShaderId m_last_vertex_shader_id { 1 };
    FragmentShaderId m_last_fragment_shader_id { 1 };
    FullShaderId m_last_full_shader_id { 1 };
    
    std::filesystem::path m_res_root;
    
    // Texture storage
    std::unordered_map<std::string, TextureId> m_texture_ids;
    std::unordered_map<TextureId, sf::Texture> m_textures;
    
    // Vertex shader storage
    std::unordered_map<std::string, VertexShaderId> m_vertex_shader_ids;
    std::unordered_map<VertexShaderId, std::shared_ptr<sf::Shader>> m_vertex_shaders;
    
    // Fragment shader storage
    std::unordered_map<std::string, FragmentShaderId> m_fragment_shader_ids;
    std::unordered_map<FragmentShaderId, std::shared_ptr<sf::Shader>> m_fragment_shaders;
    
    // Full shader storage
    std::unordered_map<std::string, FullShaderId> m_full_shader_ids;
    std::unordered_map<FullShaderId, std::shared_ptr<sf::Shader>> m_full_shaders;
    
    // Procedurally generated textures storage
    std::unordered_map<ProceduralTextureId, sf::Texture> m_procedural_textures;
};

inline std::unique_ptr<ResourceManager> g_resource_manager;

class Id
{
public:
    Id() : m_texture_type(IdTextureType::Invalid), m_id(InvalidId) {}
    Id(IdTextureType T, Id_t id) : m_texture_type(T), m_id(id) {}
    
    inline bool invalid_id() const
    {
        return m_id == InvalidId;
    }
    
    inline const sf::Texture* texture() const
    {
        switch (m_texture_type)
        {
        case IdTextureType::Invalid:
            assert(false); // This should never be reached. (Uninitialized)
            break;
        case IdTextureType::TextureId:
            return g_resource_manager->get_texture(m_id);
        case IdTextureType::ProceduralTextureId:
            return g_resource_manager->get_procedural_texture(m_id);
        default:
            return nullptr;
        }
    }
    
    inline sf::Texture* texture()
    {
        switch (m_texture_type)
        {
        case IdTextureType::Invalid:
            assert(false); // This should never be reached. (Uninitialized)
            break;
        case IdTextureType::TextureId:
            return g_resource_manager->get_texture(m_id);
        case IdTextureType::ProceduralTextureId:
            return g_resource_manager->get_procedural_texture(m_id);
        default:
            return nullptr;
        }
    }
private:
    IdTextureType m_texture_type;
    Id_t m_id;
};

#endif // RESOURCEMANAGER_H
