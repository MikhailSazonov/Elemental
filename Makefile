CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -I./VkHandlers -I./GLFW -I./

GLFW_SRC = GLFW/Window.cpp

VK_SRC = VkHandlers/Instance.cpp

SRC = main.cpp \
		Renderer.cpp \
		$(GLFW_SRC) \
		$(VK_SRC)

NAME = Test

all: $(SRC)
	g++ $(CFLAGS) -o $(NAME) $(SRC) $(LDFLAGS)

.PHONY: run clean

run: all
	./Test

clean: \
    rm -f Test