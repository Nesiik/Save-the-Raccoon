cmake_minimum_required(VERSION 3.27)
project(projet VERSION 0.1.0 LANGUAGES C)

INCLUDE(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR})

file(GLOB SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(projet ${SOURCES})
target_link_libraries(projet SDL2main SDL2 SDL2_ttf)