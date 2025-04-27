cmake_minimum_required(VERSION 3.18)
project(computicle VERSION 1.0.0 LANGUAGES CXX)

# Build type and warnings options
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE)
endif()
option(CPP_COMPUTICLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
if(CPP_COMPUTICLE_WARNINGS_AS_ERRORS)
  add_compile_options(-Werror)
endif()

include(GNUInstallDirs)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED CONFIG)
find_package(GLEW REQUIRED CONFIG)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
	# Build for Apple Silicon only to match Homebrew ARM libraries
	set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architecture for Mac" FORCE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS deployment version" FORCE)
endif()

add_executable(
	computicle
		src/main.cc
		src/timer.cc
		src/glfw/guard.cc
		src/glfw/window.cc
		src/glfw/key_watcher.cc
		src/shader/util.cc
		src/shader/wrap/compute_shader.cc
		src/shader/wrap/graphic_shader.cc
		src/buffer/frame/texture_framebuffer.cc
		src/buffer/vertex/particle_vertex_buffer.cc
		src/buffer/vertex/texture_display_vertex_buffer.cc
)

target_include_directories(computicle PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_features(computicle PRIVATE cxx_std_17)
target_compile_options(computicle PRIVATE -Wall -Wextra -pedantic -Winline)

target_link_libraries(computicle PRIVATE
	OpenGL::GL
	glfw
	GLEW::GLEW
	glm::glm
)

# Release build flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Installation rules
install(TARGETS computicle
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)