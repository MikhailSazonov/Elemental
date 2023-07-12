#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>
#include <Handler.hpp>
#include <Constants.hpp>

namespace GLFWHandlers {
    class Window : public Handler<GLFWwindow*>  {
        public:
            Window();

            ~Window();
    };
}