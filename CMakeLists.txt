cmake_minimum_required(VERSION 3.10)
project(Breakout)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver la bibliothèque SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_gfx REQUIRED)
include_directories(${SDL2_GFX_INCLUDE_DIRS})

# Inclure le dossier des en-têtes
include_directories(include)

set(SOURCE_FILES
    src/main.cpp
    src/Game.cpp
    src/Level.cpp
    src/Paddle.cpp
    src/Ball.cpp
    src/Menu.cpp
    src/bricks/Brick.cpp
    src/bricks/BasicBrick.cpp
    src/bricks/TriangularBrick.cpp
    src/bricks/HexagonalBrick.cpp
)

add_executable(Breakout ${SOURCE_FILES})
target_link_libraries(Breakout ${SDL2_LIBRARIES} SDL2_gfx SDL2_ttf)
