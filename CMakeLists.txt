cmake_minimum_required(VERSION 3.7.0)
project(projet)

#include(FindPkgConfig)
#pkg_check_modules(SDL2 REQUIRED sdl2)
#pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
#pkg_check_modules(SDL2_image REQUIRED SDL2_image)
#include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

#find_package(Freetype REQUIRED)

#add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
#add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
#add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(external)
add_executable(projet ${SOURCES})
include_directories(projet PRIVATE "external/SDL/include")
include_directories(projet PRIVATE "external/SDL_image/include")
include_directories(projet PRIVATE "external/SDL_ttf")
target_link_libraries(projet m SDL2main SDL2 SDL2_ttf SDL2_image)
