cmake_minimum_required(VERSION 3.7)
project(projet VERSION 0.1.0 LANGUAGES C)

find_package(SDL2 REQUIRED)
find_package(SDL_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIR})

add_executable(projet main.c)
target_link_libraries(projet ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARY})