#include "Window.hpp"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto wind_ptr = reinterpret_cast<Graphic::Window*>(glfwGetWindowUserPointer(window));
    wind_ptr->framebufferResized = true;
}

Graphic::Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

Graphic::Window::~Window() {
    glfwDestroyWindow(window);

    glfwTerminate();
}
