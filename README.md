# SpaceShooter SFML

A 2D space shooter game built with C++ and SFML (Simple and Fast Multimedia Library).

## Features

- **Player Movement**: Smooth character movement with WASD controls
- **Infinite World**: Dynamically loaded chunk-based world system
- **Optimized Rendering**: Efficient chunk loading/unloading based on player position
- **Cross-Platform**: Works on Windows, macOS, and Linux

## Screenshots

*Game window showing the player spaceship and tiled background*

## Prerequisites

Before building this project, make sure you have:

- **C++17** compiler (GCC, Clang, or MSVC)
- **CMake** 3.16 or higher
- **SFML** 3.0+ (or 2.5+ as fallback)

### Installing SFML

#### macOS (Homebrew)
```bash
brew install sfml
```

#### Ubuntu/Debian
```bash
sudo apt-get install libsfml-dev
```

#### Windows

**Option 1: Automatic (Recommended)**
The project will automatically download and build SFML if it's not found on your system.

**Option 2: Manual Installation**
- Download SFML from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
- Or use vcpkg: `vcpkg install sfml`

## Building

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/SpaceShooterSFML.git
   cd SpaceShooterSFML
   ```

2. **Create build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Generate build files:**
   ```bash
   cmake ..
   ```

4. **Build the project:**
   ```bash
   make
   # or on Windows with Visual Studio:
   # cmake --build .
   ```

5. **Run the game:**
   ```bash
   ./SpaceShooterSFML
   ```

### Windows Quick Start

For Windows users, there's an automated build script:

1. **Clone the repository:**
   ```cmd
   git clone https://github.com/Razvan200526/SpaceShooterSFML.git
   cd SpaceShooterSFML
   ```

2. **Run the build script:**
   ```cmd
   build_windows.bat
   ```

   This script will:
   - Check for required tools (Git, CMake)
   - Automatically detect your compiler (Visual Studio or MinGW)
   - Download and build SFML if not found
   - Build the game
   - Show you where the executable is located

**Requirements for Windows:**
- **Git** ([download here](https://git-scm.com/download/win)) - Free
- **CMake** ([download here](https://cmake.org/download/)) - Free
- **Visual Studio 2022 Community Edition** ([download here](https://visualstudio.microsoft.com/vs/community/)) - **FREE**
  - During installation, make sure to select "Desktop development with C++" workload
  - This includes the MSVC compiler and Windows SDK

**Alternative Compilers (if you prefer):**
  - Visual Studio 2019 Community Edition
  - MinGW-w64
  - Visual Studio Build Tools

## Controls

- **W, A, S, D** - Move player
- **ESC** or **Close Window** - Exit game

## Project Structure

```
SpaceShooterSFML/
├── src/
│   ├── main.cpp          # Entry point
│   ├── Game.h/cpp        # Main game class
│   ├── Player.h/cpp      # Player character
│   ├── chunk.h/cpp       # World chunk system
│   └── Projectile.h/cpp  # Projectiles (future feature)
├── assets/               # Game assets
│   ├── purple.png        # Player sprite
│   ├── background.png    # Background tile
│   └── ...
├── CMakeLists.txt        # Build configuration
└── README.md            # This file
```

## Technical Details

### Chunk System
The game uses a dynamic chunk loading system:
- World is divided into 600x600 pixel chunks
- Only chunks within 2 chunk radius of player are loaded
- Chunks are automatically loaded/unloaded as player moves
- Shared texture system for efficient memory usage

### Architecture
- **Game Loop**: Standard game loop with update/render phases
- **RAII Memory Management**: Proper resource cleanup to prevent memory leaks
- **Exception Safety**: Robust error handling for file loading and system operations

## Development

### Code Style
- Modern C++17 features
- RAII for resource management
- Const-correctness
- Exception-safe code

### Building for Development
To generate compile commands for IDE support:
```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

## Known Issues

- ~~Mutex errors on program termination~~ ✅ **FIXED**
- Background texture must be in build directory

## Future Features

- [ ] Projectile system
- [ ] Enemy spawning
- [ ] Collision detection
- [ ] Sound effects
- [ ] Particle effects
- [ ] Menu system

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **SFML Team** - For the excellent multimedia library

## Contact

Your Name - doruippu@gmail.com

Project Link: [https://github.com/yourusername/SpaceShooterSFML](https://github.com/yourusername/SpaceShooterSFML)
