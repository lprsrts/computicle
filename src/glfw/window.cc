#include "window.h"

Window::Window(const std::string& title) {
  // macOS retina and HiDPI support
  #ifdef __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    // Allow GLFW to scale window to monitor DPI
    #if defined(GLFW_SCALE_TO_MONITOR)
      glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    #endif
  #endif
   // Set up OpenGL context version and profile hints
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   // Use compatibility profile to support fixed-function pipeline shaders
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
   // Create window
   _handle = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
   if (_handle) {
       glfwMakeContextCurrent(_handle);
       // Enable experimental features for core profile
       glewExperimental = GL_TRUE;
       if (glewInit() == GLEW_OK) {
           _good = true;
       }
       glfwMakeContextCurrent(nullptr);
   }
}

Window::~Window() {
	glfwDestroyWindow(_handle);
}

bool Window::isGood() const {
	return _good;
}

int Window::getWidth() const {
	return _width;
}

int Window::getHeight() const {
	return _height;
}

KeyWatcher Window::getKeyWatcher(int key) {
	return KeyWatcher(_handle, key);
}
