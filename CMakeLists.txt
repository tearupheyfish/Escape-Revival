cmake_minimum_required(VERSION 3.26)
project(EscapeReopen)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
        add_compile_options(/utf-8)#/FA
endif ()

aux_source_directory(Escape ESCAPE)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
#find_package(globjects CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)

add_executable(Escape ${ESCAPE})
add_executable(EscapeReopen main.cpp game.cpp glpp.cpp)

target_link_libraries(Escape
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        PRIVATE Boost::boost
        )

target_link_libraries(EscapeReopen
        PRIVATE glfw glad::glad
#        globjects::globjects
)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/workdir/source ${CMAKE_BINARY_DIR}/source)