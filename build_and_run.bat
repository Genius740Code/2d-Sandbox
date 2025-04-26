@echo off
echo ===== SFML Terrain Generator =====
echo.

REM Check if SFML is installed
if not exist SFML\include\SFML (
    echo SFML not found. Running the installer...
    call download_sfml.bat
    
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to download and install SFML.
        pause
        exit /b 1
    )
)

REM Create necessary directories
if not exist obj mkdir obj
if not exist bin mkdir bin

REM Ensure the DLLs are copied first
echo Copying DLLs...
copy SFML\bin\*.dll bin

REM Copy texture directories to bin
echo Copying textures...
if exist kenney_voxel-pack (
    if not exist bin\kenney_voxel-pack mkdir bin\kenney_voxel-pack
    if not exist bin\kenney_voxel-pack\PNG mkdir bin\kenney_voxel-pack\PNG
    if not exist bin\kenney_voxel-pack\PNG\Tiles mkdir bin\kenney_voxel-pack\PNG\Tiles
    copy kenney_voxel-pack\PNG\Tiles\*.png bin\kenney_voxel-pack\PNG\Tiles\
)

REM Compile source files
echo Compiling project...
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/main.cpp -o obj/main.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/World.cpp -o obj/World.o
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/Camera.cpp -o obj/Camera.o

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Compilation failed! 
    echo.
    pause
    exit /b 1
)

echo Linking...
g++ obj/main.o obj/World.o obj/Camera.o -o bin/main.exe -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -static-libgcc -static-libstdc++

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Linking failed! 
    echo.
    pause
    exit /b 1
)

echo.
echo Running 2D Minecraft terrain generator...
echo Press Space to generate new terrain, WASD/arrow keys to scroll, Escape to exit.
echo.
cd bin
main.exe
cd ..

pause
exit /b 0 