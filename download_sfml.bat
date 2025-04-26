@echo off
echo ===== SFML Downloader and Installer =====
echo.

REM Clean up any existing SFML directories
if exist SFML rmdir /S /Q SFML
if exist SFML-2.5.1 rmdir /S /Q SFML-2.5.1

REM Download SFML 64-bit for MinGW
echo Downloading SFML 2.5.1 (64-bit, MinGW)...
echo URL: https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip

REM Use PowerShell to download the file
powershell -Command "& {Invoke-WebRequest -Uri 'https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip' -OutFile 'sfml.zip'}"

if %ERRORLEVEL% NEQ 0 (
    echo Failed to download SFML. Please check your internet connection.
    exit /b 1
)

echo Download complete! Extracting...
powershell -Command "& {Expand-Archive -Path 'sfml.zip' -DestinationPath '.' -Force}"

if %ERRORLEVEL% NEQ 0 (
    echo Failed to extract SFML.
    exit /b 1
)

echo Moving files to correct location...
xcopy /E /Y SFML-2.5.1\* SFML\

if %ERRORLEVEL% NEQ 0 (
    echo Failed to move SFML files.
    exit /b 1
)

echo Cleaning up...
del sfml.zip
rmdir /S /Q SFML-2.5.1

echo SFML 2.5.1 has been successfully installed!

echo Now creating a simplified directory structure...
mkdir SFML\lib
mkdir SFML\bin
mkdir SFML\include

echo SFML installation complete!
echo.
echo Next step: Run build_and_run.bat to compile and run your project.
echo.
pause
exit /b 0 