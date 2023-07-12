#include "Renderer.hpp"

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
    }
}
