cmake_minimum_required(VERSION 3.26)
project(EscapeReopen)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build_debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build_release)

add_compile_options(/utf-8 )#/FA

aux_source_directory(Escape ESCAPE)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(Boost REQUIRED)

message("sdl2      : ${SDL2_FOUND}")
message("sdl2-ttf  : ${SDL2_ttf_FOUND}")
message("sdl2-image: ${SDL2_image_FOUND}")

add_executable(EscapeReopen ${ESCAPE})

target_link_libraries(${PROJECT_NAME}
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        PRIVATE Boost::boost
        )

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/workdir/source ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/source)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/workdir/source ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/source)