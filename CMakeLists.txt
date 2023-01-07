cmake_minimum_required(VERSION 3.24)
project(gride)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/gui/sdl_exts.cpp src/gui/sdl_exts.hpp src/app.cpp src/app.hpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

