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
cmake -G "Visual Studio 17 2022" .. 2>nul
if errorlevel 1 (
    echo Visual Studio 2022 not found, trying Visual Studio 2019...
    cmake -G "Visual Studio 16 2019" .. 2>nul
    if errorlevel 1 (
        echo Visual Studio not found, trying MinGW...
        cmake -G "MinGW Makefiles" ..
        if errorlevel 1 (
            echo ERROR: No suitable build system found
            echo Please install Visual Studio 2019+ or MinGW-w64
            pause
            exit /b 1
        )
        echo ✓ Using MinGW Makefiles
        goto :build_mingw
    )
    echo ✓ Using Visual Studio 2019
    goto :build_vs
)
echo ✓ Using Visual Studio 2022

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
