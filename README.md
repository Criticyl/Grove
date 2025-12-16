# Grove
---

A lightweight, custom built micro-voxel engine made with OpenGL. It features a procedurally generated
environment composed of high-density voxel cubes.

This project is designed to be both a showcase and test for efficient rendering techniques rather than an actual game.

## Key Features

- **Micro-Voxel Rendering** Renders high-density (0.2cm) voxels with optimized geometry.
- **Procedural Terrain Generation**: Infinite Chunk-based terrain generation.
- **FPS-Style Camera**: A smooth First Person camera for exploration.
- **OpenGL Rendering**: Uses Modern OpenGL Implementation for rendering.

## Getting Started

### Prerequisites
- CMake (3.10+)
- C++ Compiler (GCC/Clang/MSVC etc)
- GPU with OpenGL 4.6 support

### Installation & Build
*Note: Only tested on Windows so far however this should work for Linux too.*
#### 1. **Clone the repository**
```bash
git clone https://github.com/Criticyl/Grove.git
cd grove
```
#### 2. **Build using CMake**

**For Windows:**
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
**Run the program:**
```bash
.\Release\Grove.exe
```

**For Linux/MacOS:**
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
**Run the program:**
```bash
./Grove
```
#### 3. **Installation (Optional)**
If you want to insall Grove to your system path:

**Linux / macOS:**
```bash
sudo cmake --install .
```
**Windows (Run PowerShell as Administrator):**
```bash
cmake --install .
```

## Planned Features
- **Model importing (With auto voxelization)**: Ability to import models in which will automatically be converted down to voxels.
- **World Interaction**: Interact with different structures around the map.
- **Structure Generation**: Generating structures around the map for better exploration experience.