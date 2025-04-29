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

echo Copying textures...
if exist kenney_voxel-pack (
    if not exist bin\kenney_voxel-pack mkdir bin\kenney_voxel-pack
    if not exist bin\kenney_voxel-pack\PNG mkdir bin\kenney_voxel-pack\PNG
    if not exist bin\kenney_voxel-pack\PNG\Tiles mkdir bin\kenney_voxel-pack\PNG\Tiles
    copy kenney_voxel-pack\PNG\Tiles\*.png bin\kenney_voxel-pack\PNG\Tiles\
    
    copy kenney_voxel-pack\PNG\Tiles\*.png bin\assets\textures\
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

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Compilation failed! 
    echo.
    pause
    exit /b 1
)

echo Linking...
g++ obj/main.o obj/world/World.o obj/engine/Camera.o obj/world/Chunk.o obj/world/TileManager.o obj/engine/PerlinNoise.o obj/ui/Button.o obj/ui/MenuState.o -o bin/main.exe -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -static-libgcc -static-libstdc++

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