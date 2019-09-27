project(dungeon)

set(CMAKE_CXX_STANDARD 17)

add_executable(dungeon main.cpp 
    Camera.cpp Camera.h 
    Cell.cpp Cell.h
    #CellFileParser.cpp CellFileParser.h
    CollisionEngine.cpp CollisionEngine.h
    Global.h 
    Player.cpp Player.h
    PrimitiveRectangle.h 
    Renderer.cpp Renderer.h 
    Resource.h
    ResourceManager.cpp ResourceManager.h
    Tile.cpp Tile.h 
    TileLayer.cpp TileLayer.h 
    World.cpp World.h 
    Player.cpp Player.h
    abstract/IRigidbody.cpp abstract/IRigidbody.h
)

target_link_libraries(dungeon 
    sfml-window sfml-graphics sfml-system # SFML
    stdc++fs # std::filesystem (gcc)
    pthread
    GL
    X11
)
