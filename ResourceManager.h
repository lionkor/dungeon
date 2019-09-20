#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Global.h"
#include <filesystem>
#include <assert.h>

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

typedef unsigned long long Id_t;
typedef Id_t TextureId;
typedef Id_t ProceduralTextureId;

inline static const Id_t InvalidId = 0;

enum class IdType
{
    Invalid,
    TextureId,
    ProceduralTextureId
};

class ResourceManager
{
public:
    ResourceManager(std::filesystem::path);
    
    TextureId get_texture_id(const std::string&);
    sf::Texture* get_texture(TextureId);
    
    // Caution: This function is very slow.
    ProceduralTextureId make_procedural_texture(const std::vector<TextureId>&);
    sf::Texture* get_procedural_texture(ProceduralTextureId);
    
    void load_all();
private:
    void load_textures();
    
    TextureId m_last_texture_id { 1 };
    ProceduralTextureId m_last_procedural_texture_id { 1 };
    
    std::filesystem::path m_res_root;
    
    std::unordered_map<std::string, TextureId> m_texture_ids;
    std::unordered_map<TextureId, sf::Texture> m_textures;
    
    std::unordered_map<ProceduralTextureId, sf::Texture> m_procedural_textures;
};

inline std::unique_ptr<ResourceManager> g_resource_manager;

class Id
{
public:
    Id() : m_texture_type(IdType::Invalid), m_id(InvalidId) {}
    Id(IdType T, Id_t id) : m_texture_type(T), m_id(id) {}
    
    inline bool invalid_id() const
    {
        return m_id == InvalidId;
    }
    
    inline const sf::Texture* texture() const
    {
        switch (m_texture_type)
        {
        case IdType::Invalid:
            assert(false); // This should never be reached. (Uninitialized)
            break;
        case IdType::TextureId:
            return g_resource_manager->get_texture(m_id);
        case IdType::ProceduralTextureId:
            return g_resource_manager->get_procedural_texture(m_id);
        }
    }
private:
    IdType m_texture_type;
    Id_t m_id;
};

#endif // RESOURCEMANAGER_H
