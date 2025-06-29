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

REM Display available generators
echo Checking available build systems...
cmake --help generator 2>nul | findstr "Visual Studio"
echo.

REM Create build directory
if not exist "build" mkdir build
cd build

echo Configuring project with CMake...
echo This will automatically download SFML if not found on system.
echo.

REM Try to detect Visual Studio installations more thoroughly
echo Trying to detect Visual Studio installations...

REM Check for VS 2022 using multiple methods
set VS2022_FOUND=0

REM Method 1: Check for VS 2022 using vswhere (most reliable)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    echo Checking with vswhere...
    for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -version "[17.0,18.0)" -property installationPath -latest`) do (
        if exist "%%i\Common7\IDE\devenv.exe" (
            echo ✓ Found Visual Studio 2022 at: %%i
            set VS2022_FOUND=1
        )
    )
)

REM Method 2: Check common installation paths
if %VS2022_FOUND%==0 (
    echo Checking common installation paths...
    if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" (
        echo ✓ Found Visual Studio 2022 Community
        set VS2022_FOUND=1
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" (
        echo ✓ Found Visual Studio 2022 Professional
        set VS2022_FOUND=1
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" (
        echo ✓ Found Visual Studio 2022 Enterprise
        set VS2022_FOUND=1
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\devenv.exe" (
        echo ✓ Found Visual Studio 2022 Build Tools
        set VS2022_FOUND=1
    )
)

echo.

REM Try different CMake generators based on what we found
if %VS2022_FOUND%==1 (
    echo Attempting to configure with Visual Studio 2022...

    REM Try different VS 2022 generator names
    cmake -G "Visual Studio 17 2022" -A x64 .. 2>nul
    if not errorlevel 1 (
        echo ✓ Successfully configured with Visual Studio 17 2022
        goto :build_vs
    )

    cmake -G "Visual Studio 17" -A x64 .. 2>nul
    if not errorlevel 1 (
        echo ✓ Successfully configured with Visual Studio 17
        goto :build_vs
    )

    echo ⚠ Visual Studio 2022 detected but CMake couldn't use it
)

echo Trying Visual Studio 2019...
cmake -G "Visual Studio 16 2019" -A x64 .. 2>nul
if not errorlevel 1 (
    echo ✓ Using Visual Studio 2019 (64-bit)
    goto :build_vs
)

echo Trying Visual Studio 2017...
cmake -G "Visual Studio 15 2017" -A x64 .. 2>nul
if not errorlevel 1 (
    echo ✓ Using Visual Studio 2017 (64-bit)
    goto :build_vs
)

echo Trying default generator (should pick up any available Visual Studio)...
cmake .. 2>nul
if not errorlevel 1 (
    echo ✓ Using default generator
    goto :build_auto
)

echo Visual Studio not working, trying MinGW-w64...
cmake -G "MinGW Makefiles" .. 2>nul
if not errorlevel 1 (
    echo ✓ Using MinGW-w64 Makefiles
    goto :build_mingw
)

echo Trying MSYS Makefiles...
cmake -G "MSYS Makefiles" .. 2>nul
if not errorlevel 1 (
    echo ✓ Using MSYS Makefiles
    goto :build_mingw
)

echo Trying Unix Makefiles (with MinGW/MSYS)...
cmake -G "Unix Makefiles" .. 2>nul
if not errorlevel 1 (
    echo ✓ Using Unix Makefiles
    goto :build_mingw
)

REM If all else fails, show detailed error information
echo.
echo ❌ ERROR: No suitable build system found!
echo.
echo Diagnostic information:
echo =====================
cmake --version
echo.
echo Available generators:
cmake --help generator | findstr /C:"* " | head -20
echo.
echo Visual Studio Detection Results:
echo VS2022_FOUND = %VS2022_FOUND%
echo.
echo Please try one of the following solutions:
echo.
echo   1. Make sure Visual Studio 2022 has C++ development tools installed:
echo      - Open Visual Studio Installer
echo      - Modify your VS 2022 installation
echo      - Make sure "Desktop development with C++" workload is installed
echo      - This includes MSVC compiler and Windows SDK
echo.
echo   2. Try running this from a Visual Studio Developer Command Prompt:
echo      - Start Menu → Visual Studio 2022 → x64 Native Tools Command Prompt
echo      - Navigate to your project folder
echo      - Run this script again
echo.
echo   3. Install MinGW-w64 as an alternative:
echo      - Download from: https://www.mingw-w64.org/downloads/
echo      - Or use MSYS2: https://www.msys2.org/
echo.
echo   4. Manual CMake configuration:
echo      - Try: cmake -G "Visual Studio 17 2022" -A x64 ..
echo      - Or: cmake -G "MinGW Makefiles" ..
echo.
pause
exit /b 1

:build_vs
echo.
echo Building project with Visual Studio...
cmake --build . --config Release --parallel
if errorlevel 1 (
    echo Build failed, trying Debug configuration...
    cmake --build . --config Debug --parallel
)
goto :done

:build_auto
echo.
echo Building project with default generator...
cmake --build . --config Release --parallel
if errorlevel 1 (
    echo Build failed, trying without Release config...
    cmake --build . --parallel
)
goto :done

:build_mingw
echo.
echo Building project with Make...
cmake --build . --parallel
goto :done

:done
if errorlevel 1 (
    echo.
    echo ❌ Build failed!
    echo.
    echo Try building manually:
    echo   cd build
    echo   cmake --build . --config Release
    echo.
    echo Or check the error messages above for more details.
    pause
    exit /b 1
)

echo.
echo ✅ Build successful!
echo.
echo The game executable should be in one of these locations:
if exist "Release\SpaceShooterSFML.exe" (
    echo   ✓ build\Release\SpaceShooterSFML.exe
    echo.
    echo To run the game:
    echo   cd build\Release
    echo   SpaceShooterSFML.exe
) else if exist "Debug\SpaceShooterSFML.exe" (
    echo   ✓ build\Debug\SpaceShooterSFML.exe
    echo.
    echo To run the game:
    echo   cd build\Debug
    echo   SpaceShooterSFML.exe
) else if exist "SpaceShooterSFML.exe" (
    echo   ✓ build\SpaceShooterSFML.exe
    echo.
    echo To run the game:
    echo   cd build
    echo   SpaceShooterSFML.exe
) else (
    echo   ? Could not locate the executable
    echo     Check the build directory for .exe files
)
echo.
pause
