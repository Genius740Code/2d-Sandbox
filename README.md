# 2D Minecraft Terrain Generator

This program generates 2D procedural terrain similar to Minecraft with the following features:
- Gentler rolling hills on the surface
- 3-4 layers of dirt beneath the grass
- Stone beneath the dirt
- Bedrock at the bottom of the world

## Controls
- **WASD/Arrow Keys**: Move around the world
- **Space**: Generate a new world with a different seed
- **C**: Toggle creative mode (faster movement)
- **+/=**: Zoom in
- **-**: Zoom out
- **0**: Reset zoom and view
- **Escape**: Exit the program

## Terrain Features
- Gentle hills on the surface with a natural look
- 3 layers of dirt beneath the grass
- Stone beneath the dirt layers
- Bedrock at the bottom of the world
- Uses Perlin noise for natural-looking terrain

## Textures
This program uses textures from the Kenney Voxel Pack:
- `dirt_grass.png` - For the top grass layer
- `dirt.png` - For the dirt layers
- `stone.png` - For the stone layers

## How to Build and Run

### Quick Method
1. Simply run `compile.bat` to build and run the program directly.
2. The executable `main.exe` will be created in the root directory.

### Alternative Method
1. Run `build_and_run.bat` which will compile the project and run it from the `bin` folder.
2. This script will also download SFML if you don't have it already.

### Dependencies
- The program requires SFML (Simple and Fast Multimedia Library)
- The scripts will automatically download SFML if not found

## Seeds
The program uses large seeds (between 2^50 and 2^60) to generate terrain.
The current seed is printed to the console when the program starts and whenever a new world is generated.

## Technical Details
- Uses Perlin noise for terrain height generation
- Smooth camera movement with boundary checking
- Zoom functionality to see more of the world
- Fast rendering using SFML sprites
- Optimized for performance with sprite batching
