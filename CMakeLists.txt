cmake_minimum_required(VERSION 3.7.0)
project(projet)

file(GLOB SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/external/list/list.c"
)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_image REQUIRED SDL2_image)

if(${SDL2_FOUND} AND ${SDL2_TTF_FOUND} AND ${SDL2_image_FOUND})
    message(STATUS "Found SDL2, SDL2_TTF and SDL2_image")
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})
else()
    message(STATUS "SDL2, SDL2_TTF and SDL2_image not find downloading library")
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        endif()
    endif()
    add_subdirectory(external)
    include_directories(projet PRIVATE "external/SDL/include")
    include_directories(projet PRIVATE "external/SDL_image/include")
    include_directories(projet PRIVATE "external/SDL_ttf")
endif()

add_executable(projet ${SOURCES})
include_directories(projet PRIVATE "external/list")
target_link_libraries(projet m SDL2main SDL2 SDL2_ttf SDL2_image)
