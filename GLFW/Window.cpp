#include "Window.hpp"

GLFWHandlers::Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    object_ = glfwCreateWindow(WindowConstants::WIDTH, WindowConstants::HEIGHT, "Vulkan", nullptr, nullptr);
}

GLFWHandlers::Window::~Window() {
    glfwDestroyWindow(object_);

    glfwTerminate();
}
