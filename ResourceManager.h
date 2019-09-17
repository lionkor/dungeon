#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Global.h"
#include <filesystem>

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

typedef unsigned long TextureId;

inline static const unsigned long InvalidId = 0;

class ResourceManager
{
public:
    ResourceManager(std::filesystem::path resource_root_directory);
    
    TextureId get_texture_id(const std::string& identifier);
    sf::Texture* get_texture(TextureId texture_id);
    
    void load_all();
private:
    void load_textures();
    
    TextureId m_last_id { 1 };
    
    std::filesystem::path m_res_root;
    
    std::unordered_map<std::string, TextureId> m_texture_ids;
    std::unordered_map<TextureId, sf::Texture> m_textures;
};

inline std::unique_ptr<ResourceManager> g_resource_manager;

#endif // RESOURCEMANAGER_H
