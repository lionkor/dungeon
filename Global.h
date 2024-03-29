#ifndef GLOBAL_H
#define GLOBAL_H

#include <algorithm>
#include <array>
#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

typedef unsigned char uchar;

const inline constexpr uchar g_cell_size = 50;
const inline constexpr uchar g_layer_count = 4;
const inline constexpr uchar g_tile_size = 10;
const inline constexpr uchar g_texture_size = 10;

using glm::vec2;

extern std::unique_ptr<class World> g_world;
extern std::unique_ptr<class Renderer> g_renderer;
extern std::unique_ptr<class ResourceManager> g_resource_manager;
extern std::unique_ptr<class CollisionEngine> g_collision_engine;

using Id_t = unsigned long long;
using RenderId = Id_t;
using TextureId = Id_t;
using ProceduralTextureId = Id_t;
using VertexShaderId = Id_t;
using FragmentShaderId = Id_t;
using FullShaderId = Id_t;

#include <experimental/filesystem>
#define log(x)                                                                           \
    printf("%s:%u: ", std::filesystem::path(__FILE__).filename().c_str(), __LINE__);     \
    std::cout << x << std::endl

#endif // GLOBAL_H
