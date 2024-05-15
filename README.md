# Breakout Game Project

## Description
This project is a modern implementation of the classic Breakout game using C++ and SDL2. The game features multiple levels, variable paddle control, and dynamic collision effects.

## Functionalities:
* a platform that can be moved using the keyboard
* a ball that bounces depending on its angle of impact
* bricks of different types(triangular, rectangular and hexagonal)  with different resistances
* two types of bonus(multi-ball and expand paddle)
* different grids loaded from an file


## Compilation and Running Instructions

### Prerequisites
Ensure you have `cmake` and `make`, as well as SDL2 installed on your system.

### Compiling the Game
To compile the game, follow these steps from the command line:

1. Create and navigate to the build directory:
mkdir build && cd build
2. Run CMake and build the project:
cmake .. && make

### Running the Game
After compilation, run the game by executing:
./Breakout