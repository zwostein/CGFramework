cmake_minimum_required( VERSION 2.6 )
project( cgframework )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

option( USE_INTERNAL_GLFW "Use internal glfw copy instead of system installation" OFF )
if( USE_INTERNAL_GLFW )
	set( GLFW_BUILD_DOCS FALSE CACHE BOOL "Build the GLFW documentation" )
	set( GLFW_BUILD_EXAMPLES FALSE CACHE BOOL "Build the GLFW example programs" )
	set( GLFW_BUILD_TESTS FALSE CACHE BOOL "Build the GLFW test programs" )
	set( GLFW_INSTALL FALSE CACHE BOOL "Generate installation target" )
	add_subdirectory( "glfw" )
	include_directories( "glfw/include/" )
endif()

add_executable( cgframework main.cpp Texture.cpp )
target_link_libraries( cgframework glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )

install( TARGETS cgframework RUNTIME DESTINATION bin )
