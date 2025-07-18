cmake_minimum_required(VERSION 3.16)
project(SpaceShooterSFML VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific settings for better cross-platform compatibility
if(MSVC)
    # Windows-specific settings for Visual Studio (including Community Edition)
    add_compile_options(/W4)  # High warning level
    add_compile_options(/permissive-)  # Disable non-conforming code
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)  # Disable CRT warnings
    add_compile_definitions(NOMINMAX)  # Prevent min/max macro conflicts

    # Visual Studio 2022 specific optimizations
    if(MSVC_VERSION GREATER_EQUAL 1930)  # VS 2022
        message(STATUS "Detected Visual Studio 2022 - enabling enhanced optimizations")
        add_compile_options(/std:c++17)  # Explicitly set C++17
    endif()

    # Enable static runtime linking for easier distribution
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    # GCC/Clang settings for Linux/macOS/MinGW
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(WIN32)  # MinGW on Windows
        add_compile_definitions(NOMINMAX)
    endif()
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# IMPROVED SFML HANDLING
# Option 1: Try to find SFML installed on system
find_package(SFML 3.0 COMPONENTS Graphics Window System QUIET)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS Graphics Window System QUIET)
endif()

# Option 2: If SFML not found, download and build it automatically
if(NOT SFML_FOUND)
    include(FetchContent)

    message(STATUS "SFML not found on system. Downloading and building SFML...")

    # Set SFML options BEFORE declaring the fetch content
    set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)     # We don't need audio
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)   # We don't need network
    set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)  # Don't build examples
    set(SFML_BUILD_DOC FALSE CACHE BOOL "" FORCE)       # Don't build documentation

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1  # Use SFML 3.0.1 as requested
    )

    # Make SFML available but don't populate yet
    FetchContent_GetProperties(SFML)
    if(NOT sfml_POPULATED)
        message(STATUS "Populating SFML...")
        FetchContent_Populate(SFML)

        # Add SFML subdirectory with proper configuration
        add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})

        message(STATUS "SFML downloaded and configured successfully!")

        # Set up the SFML targets manually since we're not using FetchContent_MakeAvailable
        set(SFML_FOUND TRUE)

        # Create aliases for consistency (SFML 2.6 vs 3.0 compatibility)
        if(NOT TARGET SFML::Graphics)
            add_library(SFML::Graphics ALIAS sfml-graphics)
            add_library(SFML::Window ALIAS sfml-window)
            add_library(SFML::System ALIAS sfml-system)
        endif()
    endif()
else()
    message(STATUS "Found SFML on system")
endif()

# Collect source files more explicitly
file(GLOB_RECURSE HEADER_FILES
    "src/*.h"
    "src/*.hpp"
)

file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# IMPROVED LIBRARY LINKING
# Handle both system SFML and downloaded SFML
if(TARGET sfml-graphics)
    # Downloaded SFML (targets without SFML:: prefix)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
    )
    message(STATUS "Linking with downloaded SFML libraries")
elseif(TARGET SFML::Graphics)
    # System SFML (targets with SFML:: prefix)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
    )
    message(STATUS "Linking with system SFML libraries")
else()
    message(FATAL_ERROR "No SFML targets found! This should not happen.")
endif()

# Platform-specific linking
if(WIN32)
    # On Windows, we might need additional libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
elseif(APPLE)
    # macOS-specific frameworks might be needed
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    endif()
elseif(UNIX)
    # Linux-specific libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

# Copy assets to build directory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    # Copy all assets to build directory root for easy access
    file(GLOB ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
    foreach(ASSET_FILE ${ASSET_FILES})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSET_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "Copying ${ASSET_FILE} to build directory"
        )
    endforeach()
else()
    message(WARNING "Assets directory not found. Game may not run correctly without assets.")
endif()

# Install target (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Debug information
message(STATUS "=== BUILD CONFIGURATION ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if(TARGET sfml-graphics)
    message(STATUS "SFML: Downloaded and built")
elseif(TARGET SFML::Graphics)
    message(STATUS "SFML: Found on system")
endif()
message(STATUS "===========================")
