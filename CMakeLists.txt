cmake_minimum_required(VERSION 3.10)
project(computicle LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED CONFIG)
find_package(GLEW   REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Winline -pedantic"
)

include_directories(
	src/
)

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

target_link_libraries(computicle PRIVATE
	OpenGL::GL
	glfw
	GLEW::GLEW
)