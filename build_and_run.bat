@echo off
echo ===== SFML Minecraft with Main Menu =====
echo.

if not exist SFML\include\SFML (
    echo SFML not found. Running the installer...
    call download_sfml.bat
    
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to download and install SFML.
        pause
        exit /b 1
    )
)

if not exist obj mkdir obj
if not exist obj\ui mkdir obj\ui
if not exist obj\world mkdir obj\world 
if not exist obj\engine mkdir obj\engine
if not exist bin mkdir bin
if not exist bin\assets mkdir bin\assets
if not exist bin\assets\textures mkdir bin\assets\textures

echo Copying DLLs...
copy SFML\bin\*.dll bin

echo Setting up textures...
if exist kenney_voxel-pack (
    REM First, delete any existing textures to avoid issues
    if exist bin\assets\textures\*.* del /Q bin\assets\textures\*.*
    
    REM Create necessary subdirectories
    if not exist bin\assets\textures\desert mkdir bin\assets\textures\desert
    if not exist bin\assets\textures\Ground mkdir bin\assets\textures\Ground
    if not exist bin\assets\textures\Stone mkdir bin\assets\textures\Stone
    if not exist bin\assets\textures\Ores mkdir bin\assets\textures\Ores
    if not exist bin\assets\textures\Wood mkdir bin\assets\textures\Wood
    if not exist bin\assets\textures\Wood\oak mkdir bin\assets\textures\Wood\oak
    
    REM Copy base textures
    echo Copying base textures...
    copy kenney_voxel-pack\PNG\Tiles\lava.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\water.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\snow.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\brick_grey.png bin\assets\textures\
    
    REM Copy desert tiles
    copy kenney_voxel-pack\PNG\Tiles\desert\sand.png bin\assets\textures\
    
    REM Copy Ground tiles
    copy kenney_voxel-pack\PNG\Tiles\Ground\dirt.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Ground\dirt_grass.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Ground\dirt_snow.png bin\assets\textures\
    
    REM Copy Stone tiles
    copy kenney_voxel-pack\PNG\Tiles\Stone\stone.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Stone\gravel_stone.png bin\assets\textures\
    
    REM Copy Ore tiles
    copy kenney_voxel-pack\PNG\Tiles\Ores\stone_coal.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Ores\stone_gold.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Ores\stone_silver.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Ores\stone_diamond.png bin\assets\textures\
    
    REM Copy Wood tiles
    copy kenney_voxel-pack\PNG\Tiles\Wood\oak\oak_wood.png bin\assets\textures\
    copy kenney_voxel-pack\PNG\Tiles\Wood\oak\leaves_transparent.png bin\assets\textures\
    
    REM Verify texture files were copied
    echo Verifying textures...
    dir bin\assets\textures\*.png
)

echo Compiling project...
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/main.cpp -o obj/main.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/world/World.cpp -o obj/world/World.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/engine/Camera.cpp -o obj/engine/Camera.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/world/Chunk.cpp -o obj/world/Chunk.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/world/TileManager.cpp -o obj/world/TileManager.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/engine/PerlinNoise.cpp -o obj/engine/PerlinNoise.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/ui/Button.cpp -o obj/ui/Button.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/ui/MenuState.cpp -o obj/ui/MenuState.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/ui/Slider.cpp -o obj/ui/Slider.o

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Compilation failed! 
    echo.
    pause
    exit /b 1
)

echo Linking...
g++ obj/main.o obj/world/World.o obj/engine/Camera.o obj/world/Chunk.o obj/world/TileManager.o obj/engine/PerlinNoise.o obj/ui/Button.o obj/ui/MenuState.o obj/ui/Slider.o -o bin/main.exe -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -static-libgcc -static-libstdc++

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Linking failed! 
    echo.
    pause
    exit /b 1
)

echo.
echo Running 2D Minecraft...
echo.
cd bin
main.exe
cd ..

pause
exit /b 0 