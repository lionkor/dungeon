project(dungeon)

set(CMAKE_CXX_STANDARD 17)

add_executable(dungeon "main.cpp" "Renderer.h" "Renderer.cpp")

target_link_libraries(dungeon sfml-window sfml-graphics sfml-system)
