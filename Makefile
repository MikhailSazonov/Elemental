# CFLAGS = -std=c++17 -O2
# LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# DEBUGFLAGS = -g3

# FILES = main.cpp Vulkan/*.cpp

# NAME = arcade_vulkan.so

# SHADERS = vert.spv frag.spv

# all: main.cpp
# 	g++ $(CFLAGS) -o $(NAME) $(FILES) $(LDFLAGS)
# 	./compile.sh
# 	mv $(NAME) ../../lib

# .PHONY: test clean

# clean:
# 	rm -f $(NAME) $(SHADERS)

# fclean:
# 	clean

# re: clean all


#
# agawga
#

NAME	=	arcade_vulkan.so

LDFLAGS = -std=c++17 -O2 -Wall -Wextra -Werror -shared -I../../Header

CXXFLAGS += -L../../Utils -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -fPIC

CC	=	g++

SRC	=	Vulkan/CommandManager.cpp \
		Vulkan/DebugMessenger.cpp \
		Vulkan/Instance.cpp \
		Vulkan/LogicalDevice.cpp \
		Vulkan/Operator.cpp \
		Vulkan/PhysicalDevice.cpp \
		Vulkan/Pipeline.cpp \
		Vulkan/QueueFamilyIndices.cpp \
		Vulkan/Surface.cpp \
		Vulkan/SwapChain.cpp \
		Vulkan/Utils.cpp \
		Vulkan/Vertex.cpp \
		Vulkan/VertexBuffer.cpp \
		Vulkan/Window.cpp \
		Renderer.cpp

RM = rm -f

OBJ	= $(SRC:.cpp=.o)

SHADERS = vert.spv frag.spv

all: $(OBJ)
	$(CC) $(LDFLAGS) $(CXXFLAGS) $(OBJ) -o $(NAME)
	./compile.sh
	mkdir -p ../lib
	mv $(NAME) ../lib

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) ../lib/$(NAME)
	$(RM) $(SHADERS)

re: fclean all

.PHONY: all clean fclean re
