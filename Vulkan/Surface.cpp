#include "Surface.hpp"

#include <iostream>

Graphic::Surface::Surface(Graphic::Instance& instance, Graphic::Window& window)
    : instance(instance),
      window(window) {
    if (glfwCreateWindowSurface(instance.getRawInstance(), window.getRawWindow(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

Graphic::Surface::~Surface() {
    vkDestroySurfaceKHR(instance.getRawInstance(), surface, nullptr);
}
