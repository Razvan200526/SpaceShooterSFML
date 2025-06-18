cmake_minimum_required(VERSION 3.10)
project(Game_Sfml)

set(CMAKE_CXX_STANDARD 17)


find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
file(GLOB SRC_FILES src/*.cpp)
add_executable(Game_Sfml ${SRC_FILES})

target_link_libraries(Game_Sfml PRIVATE sfml-graphics sfml-window sfml-system)
