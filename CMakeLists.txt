cmake_minimum_required(VERSION 3.26)
project(EscapeReopen)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(/utf-8 )#/FA

aux_source_directory(Escape ESCAPE)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(EscapeReopen ${ESCAPE})

target_link_libraries(${PROJECT_NAME}
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        PRIVATE Boost::boost
        )

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/workdir/source ${CMAKE_BINARY_DIR}/source)