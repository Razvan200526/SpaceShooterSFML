@echo off
echo ===============================================
echo SpaceShooter SFML - Windows Build Script
echo ===============================================
echo.

REM Check if git is available
git --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Git is not installed or not in PATH
    echo Please install Git from: https://git-scm.com/download/win
    pause
    exit /b 1
)

REM Check if cmake is available
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from: https://cmake.org/download/
    pause
    exit /b 1
)

echo ✓ Git found
echo ✓ CMake found
echo.

REM Create build directory
if not exist "build" mkdir build
cd build

echo Configuring project with CMake...
echo This will automatically download SFML if not found on system.
echo.

REM Configure with CMake (this will auto-download SFML if needed)
echo Trying Visual Studio 2022 (Community/Professional/Enterprise)...
cmake -G "Visual Studio 17 2022" -A x64 .. 2>nul
if errorlevel 1 (
    echo.
    echo Visual Studio 2022 not found, trying Visual Studio 2019...
    cmake -G "Visual Studio 16 2019" -A x64 .. 2>nul
    if errorlevel 1 (
        echo.
        echo Visual Studio not found, trying MinGW-w64...
        cmake -G "MinGW Makefiles" ..
        if errorlevel 1 (
            echo.
            echo ❌ ERROR: No suitable build system found!
            echo.
            echo Please install one of the following:
            echo   1. Visual Studio 2022 Community Edition (FREE)
            echo      Download: https://visualstudio.microsoft.com/vs/community/
            echo      Make sure to install "Desktop development with C++" workload
            echo.
            echo   2. Visual Studio 2019 Community Edition
            echo      Download: https://visualstudio.microsoft.com/vs/older-downloads/
            echo.
            echo   3. MinGW-w64
            echo      Download: https://www.mingw-w64.org/downloads/
            echo.
            pause
            exit /b 1
        )
        echo ✓ Using MinGW-w64 Makefiles
        goto :build_mingw
    )
    echo ✓ Using Visual Studio 2019 (64-bit)
    goto :build_vs
)
echo ✓ Using Visual Studio 2022 (64-bit)

:build_vs
echo.
echo Building project with Visual Studio...
cmake --build . --config Release
goto :done

:build_mingw
echo.
echo Building project with MinGW...
cmake --build .
goto :done

:done
if errorlevel 1 (
    echo.
    echo ❌ Build failed!
    pause
    exit /b 1
)

echo.
echo ✅ Build successful!
echo.
echo The game executable is located at:
if exist "Release\SpaceShooterSFML.exe" (
    echo   build\Release\SpaceShooterSFML.exe
) else (
    echo   build\SpaceShooterSFML.exe
)
echo.
echo To run the game:
if exist "Release\SpaceShooterSFML.exe" (
    echo   cd build\Release
    echo   SpaceShooterSFML.exe
) else (
    echo   cd build
    echo   SpaceShooterSFML.exe
)
echo.
pause
