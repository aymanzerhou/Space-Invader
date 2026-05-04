# Space Invaders

A classic Space Invaders game with multiple implementation options including C++/SFML, Python/Pygame, and Python/Tkinter versions.

## Features

- Player spaceship that can move left/right and shoot
- Grid of alien invaders that move in formation
- Collision detection between bullets and aliens
- Score tracking system
- Win/lose conditions with restart functionality
- Smooth animations and controls

## Controls

- **Left Arrow**: Move spaceship left
- **Right Arrow**: Move spaceship right
- **Space**: Shoot bullets
- **R**: Restart game (when game over)

## Files Overview

### C++ Version (SFML)
- **`main.cpp`** - Complete C++ implementation using SFML graphics library. Features custom-shaped player and aliens, starfield background, collision detection, scoring system, and game over conditions.
- **`CMakeLists.txt`** - CMake configuration file for building the C++ version with SFML dependencies.

### Python Versions
- **`space_invaders.py`** - Python implementation using Pygame library. Full-featured version with enhanced graphics and smooth animations.
- **`space_invaders_tkinter.py`** - Python implementation using tkinter (built-in Python library). No external dependencies required beyond Python standard library.
- **`space_invaders_console.cpp`** - Console-based C++ version using Windows API for terminal graphics (alternative to SFML version).
- **`space_invaders.bat`** - Batch file attempt at creating a pure Windows batch version (incomplete/complex).

### Build/Run Scripts
- **`compile.bat`** - Batch script for compiling the C++ version using Visual Studio Build Tools.

## Running the Game

### Option 1: Python/Tkinter Version (Recommended - No Dependencies)
```bash
python space_invaders_tkinter.py
```
*This version uses tkinter which comes pre-installed with Python, so no additional packages needed.*

### Option 2: Python/Pygame Version (Enhanced Graphics)
```bash
# First install pygame (one-time setup)
pip install pygame

# Then run the game
python space_invaders.py
```

### Option 3: C++/SFML Version (Requires Compilation)

#### Prerequisites
- C++17 compatible compiler (Visual Studio Build Tools, g++, etc.)
- SFML 2.5 or higher graphics library
- CMake (optional, for easier building)

#### Method A: Using CMake (Recommended)
```bash
# Install SFML first:
# Windows: Download from https://www.sfml-dev.org/download.php
# Linux: sudo apt-get install libsfml-dev
# macOS: brew install sfml

# Build and run:
mkdir build
cd build
cmake ..
cmake --build .
./SpaceInvaders
```

#### Method B: Manual Compilation
```bash
g++ main.cpp -o SpaceInvaders -lsfml-graphics -lsfml-window -lsfml-system
./SpaceInvaders
```

#### Method C: Using Visual Studio Build Tools (Windows)
```bash
# Run the provided batch script
.\compile.bat

# Or manually:
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc main.cpp /Fe:SpaceInvaders.exe
SpaceInvaders.exe
```

## How to Play

1. Use arrow keys to move your spaceship
2. Press space to shoot at the aliens
3. Destroy all aliens to win
4. Don't let the aliens reach your position
5. Each alien destroyed gives you 10 points
6. Press R to restart after game over

## Game Rules

- Aliens move horizontally and drop down when reaching screen edges
- Game ends when all aliens are destroyed (win) or aliens reach player (lose)
- You can only shoot one bullet at a time (with a short cooldown)

## System Requirements

### Python/Tkinter Version
- Python 3.x (tkinter included with Python)
- OS: Windows, Linux, or macOS
- Memory: Minimal requirements
- Storage: Less than 50KB

### Python/Pygame Version
- Python 3.x
- Pygame library (install via pip)
- OS: Windows, Linux, or macOS  
- Graphics: Any system with basic graphics support
- Memory: Minimal requirements
- Storage: Less than 100KB

### C++/SFML Version
- C++17 compatible compiler
- SFML 2.5 or higher graphics library
- OS: Windows, Linux, or macOS
- Graphics: Any system with OpenGL support
- Memory: Minimal requirements
- Storage: Less than 1MB

## Quick Start

**For immediate gameplay without any setup:**
```bash
python space_invaders_tkinter.py
```

This is the easiest way to start playing as it requires no additional package installations!
