project(cell_editor)

set(CMAKE_CXX_STANDARD 17)

add_executable(cell_editor main.cpp
    Renderer.cpp Renderer.h
)

target_link_libraries(cell_editor 
    sfml-window sfml-graphics sfml-system # SFML
    stdc++fs # std::filesystem (gcc)
)
