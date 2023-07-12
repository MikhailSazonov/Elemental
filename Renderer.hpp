#pragma once

#include <Window.hpp>
#include <Instance.hpp>

class Application {
    public:
        void run();

    private:
        // using namespace GLFWHandlers;
        // using namespace VkHandlers;

        GLFWHandlers::Window window_;
        VkHandlers::Instance instance_;
};
