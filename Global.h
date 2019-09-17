#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <map>
#include <unordered_map>
#include <cstdio>


#include "Vector2/Vector2.hpp"

typedef unsigned char uchar;

const inline constexpr uchar g_cell_size    = 50;
const inline constexpr uchar g_layer_count  = 4;
const inline constexpr uchar g_tile_size    = 10;
const inline constexpr uchar g_texture_size = 10;

extern std::unique_ptr<class World> g_world;
extern std::unique_ptr<class Renderer> g_renderer;
extern std::unique_ptr<class ResourceManager> g_resource_manager;

#include <filesystem>
#define log(x) printf("%s:%u: ", std::filesystem::path(__FILE__).filename().c_str(), __LINE__); std::cout << x << std::endl

#endif // GLOBAL_H
