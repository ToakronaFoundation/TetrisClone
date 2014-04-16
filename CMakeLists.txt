cmake_minimum_required(VERSION 2.8)

project (tetrisclone)

file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE HEADERS "*.h")

set (INCLUDE_DIRS "src")
foreach (_headerFile ${HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

include_directories(${INCLUDE_DIRS})

ADD_DEFINITIONS(
	-std=c99
	-Wall
	-O3
)

add_executable(tetrisclone src/Main)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(tetrisclone ${GLFW_LIBRARIES})

pkg_search_module(OPENGL REQUIRED gl)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(tetrisclone ${OPENGL_LIBRARIES})
