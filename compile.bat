@echo off
echo ===== Building 2D Minecraft Terrain Generator =====
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

REM Check if texture directory exists
if not exist kenney_voxel-pack\PNG\Tiles (
    echo Texture directory not found!
    echo Please make sure kenney_voxel-pack\PNG\Tiles exists and contains the necessary textures.
    pause
    exit /b 1
)

REM Create necessary directories
if not exist obj mkdir obj

REM Ensure the DLLs are copied to the root directory
echo Copying DLLs...
copy SFML\bin\*.dll .

REM Compile the main cpp file directly
echo Compiling project...
g++ -Wall -Wextra -std=c++17 -O2 -I./SFML/include -c src/main.cpp -o obj/main.o

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Compilation failed! 
    echo.
    pause
    exit /b 1
)

echo Linking...
g++ obj/main.o -o main.exe -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -static-libgcc -static-libstdc++

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Linking failed! 
    echo.
    pause
    exit /b 1
)

echo.
echo Successfully compiled main.exe!
echo Press any key to run the program, or close this window to run it later.
echo.
pause

main.exe 