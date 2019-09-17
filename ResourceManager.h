#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>

#include "Resource.h"
#include "Global.h"
#include <filesystem>

class ResourceManager
{
public:
    ResourceManager(std::filesystem::path resource_root_directory);
    
    sf::Texture* get_texture(const std::string& identifier);
    
    void load_all();
private:
    void load_textures();
    
    std::filesystem::path m_res_root;
    std::unordered_map<std::string, sf::Texture> m_textures;
};

inline std::unique_ptr<ResourceManager> g_resource_manager;
inline sf::Texture* g_invalid_texture = nullptr;

#endif // RESOURCEMANAGER_H
