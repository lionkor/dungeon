#ifndef CELLFILEPARSER_H
#define CELLFILEPARSER_H

#include "Global.h"

class CellFileParser
{
public:
    static std::array<std::array<class Tile, g_cell_size>, g_cell_size>&&
    parse(const std::filesystem::path& file_path);
};

#endif // CELLFILEPARSER_H
