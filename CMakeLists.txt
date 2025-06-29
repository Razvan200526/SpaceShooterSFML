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
    # Windows-specific settings for Visual Studio
    add_compile_options(/W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang settings for Linux/macOS
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Option 1: Try to find SFML installed on system
find_package(SFML 3.0 COMPONENTS Graphics Window System)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS Graphics Window System)
endif()

# Option 2: If SFML not found, download and build it automatically
if(NOT SFML_FOUND)
    include(FetchContent)
    
    message(STATUS "SFML not found on system. Downloading and building SFML...")
    
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1  # Use stable version
    )
    
    # Configure SFML options
    set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)     # We don't need audio
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)   # We don't need network
    set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)  # Don't build examples
    set(SFML_BUILD_DOC FALSE CACHE BOOL "" FORCE)       # Don't build documentation
    
    FetchContent_MakeAvailable(SFML)
    
    message(STATUS "SFML downloaded and configured successfully!")
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

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

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
