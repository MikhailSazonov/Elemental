#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>
#include <Handler.hpp>
#include <vector>
#include <iostream>

namespace VkHandlers {
    class Instance : public Handler<VkInstance> {
        public:
            Instance();

            ~Instance();

        private:
            void CheckExt();
    };
}
