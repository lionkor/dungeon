#include "CellFileParser.h"
#include "Cell.h"

std::array<std::array<Tile, g_cell_size>, g_cell_size>&&
CellFileParser::parse(const std::filesystem::path& file_path)
{
    std::array<std::array<Tile, g_cell_size>, g_cell_size> arr{};
    /*
     * Parses custom .cell format.
     * .cell format works like this:
     *
     */
}
