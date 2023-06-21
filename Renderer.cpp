#include "Renderer.hpp"

static bool left_pressed{false};
static bool right_pressed{false};
static bool escape_pressed{false};
static bool q_pressed{false};
static bool e_pressed{false};

static void KeyCallback(GLFWwindow *, int key, int, int action, int) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
            left_pressed = true;
        }
        if (key == GLFW_KEY_RIGHT) {
            right_pressed = true;
        }
        if (key == GLFW_KEY_ESCAPE) {
            escape_pressed = true;
        }
        if (key == GLFW_KEY_Q) {
            q_pressed = true;
        }
        if (key == GLFW_KEY_E) {
            e_pressed = true;
        }
    }
}

static int getColor(unsigned color)
{
    if (color == Game::Display::AObject::BackupColor::WHITE)
        return 0xFFFFFF;
    if (color == Game::Display::AObject::BackupColor::BLACK)
        return 0x000000;
    if (color == Game::Display::AObject::BackupColor::RED)
        return 0xFF0000;
    if (color == Game::Display::AObject::BackupColor::GREEN)
        return 0x00FF00;
    if (color == Game::Display::AObject::BackupColor::BLUE)
        return 0x0000FF;
    if (color == Game::Display::AObject::BackupColor::YELLOW)
        return 0xFFFF00;
    if (color == Game::Display::AObject::BackupColor::MAGENTA)
        return 0xFF00FF;
    if (color == Game::Display::AObject::BackupColor::CYAN)
        return 0x00FFFF;
    return 0x000000;
}

Graphical::Renderer::Renderer()
    :
    #ifndef NDEBUG
    messenger(instance),
    #endif
    surface(instance, window)
    , physical_device(instance, indices, surface)
    , logical_device(physical_device, indices)
    , swap_chain(physical_device, logical_device, surface, indices, window)
    , pipeline(logical_device, swap_chain.getSwapChainExtent(), swap_chain.getImageFormat(), swap_chain.getImageViews())
    , manager(indices, logical_device, pipeline.getGraphicsPipeline(),
    pipeline.getViewport(), pipeline.getScissor(), pipeline.getRenderPass(), pipeline.getSwapChainFrameBuffers(),
    swap_chain.getSwapChainExtent())
    , vertex_buffer(logical_device, physical_device)
    , operator_(logical_device, window)
{
    glfwSetKeyCallback(window.getRawWindow(), KeyCallback);
}

bool Graphical::Renderer::init(Core::IMediator *mediator) {
    mediator_ = mediator;
    return true;
}

void Graphical::Renderer::drawTriangle(int32_t x1, int32_t y1, int32_t x2,
int32_t y2, int32_t x3, int32_t y3, int32_t color) {
    float color_r = ((color & 0xFF0000) >> 16) / 256.0f;
    float color_g = ((color & 0x00FF00) >> 8) / 256.0f;
    float color_b = (color & 0x0000FF) / 256.0f;

    vertex_buffer.pushVertex({{x1 / 100.0f, y1 / 100.0f}, {color_r, color_g, color_b}});
    vertex_buffer.pushVertex({{x2 / 100.0f, y2 / 100.0f}, {color_r, color_g, color_b}});
    vertex_buffer.pushVertex({{x3 / 100.0f, y3 / 100.0f}, {color_r, color_g, color_b}});
}

void Graphical::Renderer::drawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color) {
    drawTriangle(x2, y2, x2, y1, x1, y1, color);
    drawTriangle(x1, y2, x2, y2, x1, y1, color);
}

void Graphical::Renderer::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color) {
    if (std::abs(x1 - x2) > std::abs(y1 - y2)) {
        drawTriangle(x1, y1 - 1, x2, y2 - 1, x1, y1, color);
        drawTriangle(x2, y2 - 1, x2, y2, x1, y1, color);
    } else {
        drawTriangle(x2 + 1, y2, x1 + 1, y1, x1, y1, color);
        drawTriangle(x2, y2, x2 + 1, y2, x1, y1, color);
    }
}

// Under construction.

void Graphical::Renderer::drawCircle(int32_t x, int32_t y, int32_t r, int32_t color, int32_t division_factor = 36) {
    ++r;
    float angle = (360.0f / (division_factor * 2)) * (M_PI / 180);
    Math::Matrix<3, 2> triangle_matrix;
    Math::Matrix<2, 2> rotation_matrix;
    Math::Matrix<3, 2> shift_matrix;

    // init triangle matrix
    triangle_matrix[0][0] = x + r * sin(angle);
    triangle_matrix[0][1] = y - r * cos(angle);
    triangle_matrix[1][0] = x - r * sin(angle);
    triangle_matrix[1][1] = y - r * cos(angle);
    triangle_matrix[2][0] = x;
    triangle_matrix[2][1] = y;

    // init rotation_matrix
    rotation_matrix[0][0] = cos(angle * 2);
    rotation_matrix[0][1] = sin(angle * 2);
    rotation_matrix[1][0] = -sin(angle * 2);
    rotation_matrix[1][1] = cos(angle * 2);

    // init shift matrix
    for (size_t i = 0; i < 3; ++i) {
        shift_matrix[i][0] = x;
        shift_matrix[i][1] = y;
    }

    for (size_t i = 0; i < division_factor; ++i) {
        drawTriangle(triangle_matrix[0][0], triangle_matrix[0][1],
                        triangle_matrix[1][0], triangle_matrix[1][1],
                        triangle_matrix[2][0], triangle_matrix[2][1], color);
        triangle_matrix -= shift_matrix;
        triangle_matrix *= rotation_matrix;
        triangle_matrix += shift_matrix;
    }
}

void Graphical::Renderer::draw(std::vector<std::unique_ptr<g::Display::AObject>> &objs) {
    if (!window.shouldClose()) {
        for (auto &obj : objs)
        {
            int type = obj->getType();
            switch (type)
            {
            case Game::Display::AObject::Type::CIRCLE:
                draw(*dynamic_cast<Game::Display::Circle *>(obj.get()));
                break;
            case Game::Display::AObject::Type::RECTANGLE:
                draw(*dynamic_cast<Game::Display::Rectangle *>(obj.get()));
                break;
            case Game::Display::AObject::Type::TEXT:
                draw(*dynamic_cast<Game::Display::Text *>(obj.get()));
                break;
            case Game::Display::AObject::Type::LINE:
                draw(*dynamic_cast<Game::Display::Line *>(obj.get()));
                break;
            case Game::Display::AObject::Type::SPRITE:
                draw(*dynamic_cast<Game::Display::Sprite *>(obj.get()));
                break;
            default:
                throw Game::Display::AObject::AObjectUnknownException();
            }
        }


        vertex_buffer.fillBuffer();
        if (!operator_.drawFrame(swap_chain, manager, vertex_buffer)) {
            recreateSwapChain();
        }
        vertex_buffer.resetVertices();
    }
}

bool Graphical::Renderer::draw(g::Display::Circle &obj) {
    drawCircle(obj.pos.x, obj.pos.y, obj.radius, getColor(obj.backupColor));
    return true;
}

bool Graphical::Renderer::draw(g::Display::Line &obj) {
    drawLine(obj.pos.x, obj.pos.y, obj.x2, obj.y2, getColor(obj.backupColor));
    return true;
}

bool Graphical::Renderer::draw(g::Display::Rectangle &obj) {
    drawRectangle(obj.pos.x, obj.pos.y, obj.pos.x + obj.width, obj.pos.y - obj.height, getColor(obj.backupColor));
    return true;
}

void Graphical::Renderer::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.getRawWindow(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window.getRawWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(logical_device.getRawDevice());

    swap_chain.createSwapChain();

    pipeline.createFrameBuffers(swap_chain.getSwapChainExtent(), swap_chain.getImageViews());
}

g::Display::AObject::coordinates Graphical::Renderer::pos_from_percent(g::Display::AObject::coordinates pos) {
    Game::Display::AObject::coordinates size;

    if (mediator_->GetPositionMode() == false)
        return pos;
    size.x = (pos.x * 100) / 100;
    size.y = (pos.y * 100) / 100;
    return size;
}

void Graphical::Renderer::putKey(Game::Events::KeyboardEvent::KeyNum key) {
    std::unique_ptr<g::Events::AEvents> event = std::make_unique<Game::Events::KeyboardEvent>();
    event->key = key;
    event->type = Game::Events::KeyboardEvent::Type::KeyPressed;
    events.push_back(std::move(event));
}

void Graphical::Renderer::event_loop() {
    glfwPollEvents();
    if (left_pressed) {
        left_pressed = false;
        putKey(Game::Events::KeyboardEvent::KeyNum::LEFT_ARROW);
    }
    if (right_pressed) {
        right_pressed = false;
        putKey(Game::Events::KeyboardEvent::KeyNum::RIGHT_ARROW);
    }
    if (escape_pressed) {
        escape_pressed = false;
        putKey(Game::Events::KeyboardEvent::KeyNum::ESCAPE);
    }
    if (q_pressed) {
        q_pressed = false;
        putKey(Game::Events::KeyboardEvent::KeyNum::KEY_Q);
    }
    if (e_pressed) {
        e_pressed = false;
        putKey(Game::Events::KeyboardEvent::KeyNum::KEY_E);
    }
}

bool Graphical::Renderer::toClose() {
    vkDeviceWaitIdle(logical_device.getRawDevice());
    delete this;
    return true;
}

std::vector<std::unique_ptr<g::Events::AEvents>> Graphical::Renderer::GetEvents() {
    auto events_to_process = std::move(events);
    events.clear();
    return events_to_process;
}


extern "C"
{
    Core::IMod *GetModule(Core::IMediator *mediator)
    {
        Graphical::IGraphic *obj = new Graphical::Renderer();
        obj->init(mediator);
        return obj;
    }
    Core::ModInfo GetHeader()
    {
        Core::ModInfo info;
        info.type = Core::ModInfo::Type::GRAPHIC;
        info.name = "Vulkan";
        return info;
    }
}
