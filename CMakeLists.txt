cmake_minimum_required(VERSION 2.8)

# Options
#   Usage: cmake -D<option name>=<value> <directory>
option(USE_SHARED_GLFW "Using shared GLFW library" ON)

project (tetrisclone C)

# Find all source and header files
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "src/*.h")

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
)

# Extra warnings for gcc
#if(CMAKE_C_COMPILER matches gcc and CMAKE_BUILD_TYPE matches Debug)
#    add_definitions(
#		-ftrapv
#		-Wundef
#		-Wpointer-arith
#		-Wcast-align
#		-Wwrite-strings
#		-Wcast-qual
#		-Wswitch-default
#		-Wunreachable-code
#		-Wfloat-equal
#		-Wuninitialized
#		-Wignored-qualifiers
#		-Wsuggest-attribute=pure
#		-Wsuggest-attribute=const
#	)
#endif()

# Compile and assemble the executable with all found source files
add_executable(tetrisclone ${SOURCES})

# Find and use pkgconfig
find_package(PkgConfig REQUIRED)

# GLFW3 linking
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
if (USE_SHARED_GLFW)
	target_link_libraries(tetrisclone ${GLFW_LIBRARIES})
else()
	target_link_libraries(tetrisclone ${GLFW_STATIC_LIBRARIES})
endif()

# OpenGL linking
pkg_search_module(OPENGL REQUIRED gl)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(tetrisclone ${OPENGL_LIBRARIES})
