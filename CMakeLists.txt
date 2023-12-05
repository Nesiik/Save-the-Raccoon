cmake_minimum_required(VERSION 3.7.0)
project(projet)

file(GLOB SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_subdirectory(external)
add_executable(projet ${SOURCES})
include_directories(projet PRIVATE "external/SDL/include")
include_directories(projet PRIVATE "external/SDL_image/include")
include_directories(projet PRIVATE "external/SDL_ttf")
target_link_libraries(projet m SDL2main SDL2 SDL2_ttf SDL2_image)
