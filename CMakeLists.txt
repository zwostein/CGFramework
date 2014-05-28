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

option( USE_INTERNAL_GLM "Use internal GLM copy instead of system installation" OFF )
if( USE_INTERNAL_GLM )
	include_directories( "glm/" )
endif()

option( HEADLESS "Disables window creation for headless operation" OFF )
if( HEADLESS )
	add_definitions( -DHEADLESS )
endif()

add_executable( example_imagebuffer main.imagebuffer.cpp Pixelator.cpp Texture.cpp BitmapFile.cpp ImageBuffer.cpp )
target_link_libraries( example_imagebuffer glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )

add_executable( example_modelviewer main.modelviewer.cpp Pixelator.cpp Texture.cpp BitmapFile.cpp ImageBuffer.cpp )
target_link_libraries( example_modelviewer glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )
