#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>
#include <array>
#include <filesystem>
#include "../Vector2/Vector2.hpp"

const static constexpr unsigned char g_cell_size = 50;
const static constexpr unsigned char g_texture_size = 10;
const static constexpr unsigned char g_layer_count = 4;
const static constexpr unsigned char g_tile_size = 10;

extern std::unique_ptr<class Renderer> g_renderer;

#endif // GLOBAL_H
