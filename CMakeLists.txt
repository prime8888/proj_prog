cmake_minimum_required(VERSION 3.10)
project(Breakout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver la bibliothèque SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Inclure le dossier des en-têtes
include_directories(include)

set(SOURCE_FILES
    src/main.cpp
    src/Game.cpp
    src/Level.cpp
    src/Paddle.cpp
    src/Ball.cpp
    src/Utilities.cpp
    src/bricks/BasicBrick.cpp  # Ensure this line is present
)

add_executable(Breakout ${SOURCE_FILES})
target_link_libraries(Breakout ${SDL2_LIBRARIES} SDL2_gfx SDL2_ttf)
