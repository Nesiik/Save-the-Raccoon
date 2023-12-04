cmake_minimum_required(VERSION 3.27)
project(projet)

include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_image REQUIRED SDL2_image)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(projet ${SOURCES})
target_link_libraries(projet m SDL2main SDL2 SDL2_ttf SDL2_image)
