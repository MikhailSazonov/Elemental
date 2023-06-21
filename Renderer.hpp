// #define NDEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>

#include "Vulkan/Window.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/DebugMessenger.hpp"
#include "Vulkan/PhysicalDevice.hpp"
#include "Vulkan/LogicalDevice.hpp"
#include "Vulkan/QueueFamilyIndices.hpp"
#include "Vulkan/Surface.hpp"
#include "Vulkan/Utils.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Pipeline.hpp"
#include "Vulkan/CommandManager.hpp"
#include "Vulkan/Operator.hpp"
#include "Vulkan/Vertex.hpp"
#include "Vulkan/VertexBuffer.hpp"

#include "Matrix.hpp"

#include <iostream>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "../Headers/Abstracts/IGraphic.hpp"

namespace Graphical {

class Renderer : public IGraphic {
public:
    Renderer();

    void drawTriangle(int32_t x1, int32_t y1, int32_t x2,
    int32_t y2, int32_t x3, int32_t y3, int32_t color);

    void drawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color);

    void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color);

    void drawCircle(int32_t x, int32_t y, int32_t r, int32_t color, int32_t division_factor);

public:
    inline bool CheckEvents() { return !events.empty(); }

    bool init(Core::IMediator *mediator) override;

    std::vector<std::unique_ptr<g::Events::AEvents>> GetEvents();

    bool draw(g::Display::Circle &objs);

    bool draw(g::Display::Line &objs);

    bool draw(g::Display::Rectangle &objs);

    // TODO
    bool draw(g::Display::Text &objs) { return true; }

    bool draw(g::Display::Sprite &objs) { return true; }

    void draw(std::vector<std::unique_ptr<g::Display::AObject>> &objs);

    bool refresh_screen() { return true; }

    bool clear_screen() { return true; }

    bool toClose();

    void event_loop();

    bool load(const std::string &type, const std::string &path) { return true; }
    void unload(const std::string &type, const std::string &path) {}
    void unloadAll() {}
    void playSound(g::Sound &sound) {}

    g::Display::AObject::coordinates pos_from_percent(g::Display::AObject::coordinates pos);

private:
    void recreateSwapChain();

    void putKey(Game::Events::KeyboardEvent::KeyNum key);

private:
    Graphic::Window window;
    Graphic::Instance instance;
    Graphic::Surface surface;
    Graphic::QueueFamilyIndices indices;

    #ifndef NDEBUG
    Graphic::DebugMessenger messenger;
    #endif

    Graphic::PhysicalDevice physical_device;
    Graphic::LogicalDevice logical_device;

    Graphic::SwapChain swap_chain;
    Graphic::Pipeline pipeline;
    Graphic::CommandManager manager;

    Graphic::Operator operator_;
    Graphic::VertexBuffer vertex_buffer;

    Core::IMediator *mediator_;

    std::vector<std::unique_ptr<Game::Events::AEvents>> events;
};

}