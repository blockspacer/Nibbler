CC := clang++

TARGET := Nibbler

SDL2_LOC := $(shell brew --prefix sdl2)
SDL2_INC := $(SDL2_LOC)/include/SDL2/
SDL2_LINK := -L $(SDL2_LOC)/lib/ -lSDL2

SDL2_IMAGE_LOC := $(shell brew --prefix sdl2_image)
SDL2_IMAGE_INC := $(SDL2_IMAGE_LOC)/include/SDL2/
SDL2_IMAGE_LINK := -L $(SDL2_IMAGE_LOC)/lib/ -lSDL2_image

SDL2_TTF_LOC := $(shell brew --prefix sdl2_ttf)
SDL2_TTF_INC := $(SDL2_TTF_LOC)/include/SDL2/
SDL2_TTF_LINK := -L $(SDL2_TTF_LOC)/lib/ -lSDL2_ttf

SDL2_MIXER_LOC := $(shell brew --prefix sdl2_mixer)
SDL2_MIXER_INC := $(SDL2_MIXER_LOC)/include/SDL2/
SDL2_MIXER_LINK := -L $(SDL2_MIXER_LOC)/lib/ -lSDL2_mixer

SFML_LOC := $(shell brew --prefix sfml)
SFML_INC := $(SFML_LOC)/include/
SFML_LINK := -Wl,-rpath $(SFML_LOC)/lib \
	-L $(SFML_LOC)/lib/ -lsfml-graphics \
	-L $(SFML_LOC)/lib/ -lsfml-window \
	-L $(SFML_LOC)/lib/ -lsfml-system \
	-L $(SFML_LOC)/lib/ -lsfml-network \

GLEW_LOC := $(shell brew --prefix glew)
GLEW_INC := $(GLEW_LOC)/include/
GLEW_LINK := -L $(GLEW_LOC)/lib/ -lGLEW

GLM_LOC := $(shell brew --prefix glm)
GLM_INC := $(GLM_LOC)/include/

CFLAGS := -std=c++11 -Wall -Werror -Wextra -Wfatal-errors -g -fsanitize=address

INCLUDES := includes/
HEADERS := -I $(INCLUDES) \
	-I $(SDL2_INC) \
	-I $(SDL2_IMAGE_INC) \
	-I $(SDL2_TTF_INC) \
	-I $(SDL2_MIXER_INC) \
	-I $(SFML_INC) \
	-I $(GLEW_INC) \
	-I $(GLM_INC)

SRCSDIR := srcs/
SRCS := \
AudioManager.cpp \
Board.cpp \
Cell.cpp \
Client.cpp \
Enemy.cpp \
EnemyCell.cpp \
FoodCell.cpp \
Model.cpp \
NetworkEntity.cpp \
Nibbler.cpp \
NibblerException.cpp \
OpenGLModule.cpp \
ParsingException.cpp \
Player.cpp \
ResourceManager.cpp \
SDLModule.cpp \
SFMLModule.cpp \
Server.cpp \
Shader.cpp \
Snake.cpp \
SnakeCell.cpp \
direction.cpp \
main.cpp

OBJDIR := objs/
OBJS := $(addprefix $(OBJDIR), $(SRCS:.cpp=.o))

MY_SFML_LIB := myLibSFML.so
MY_SDL_LIB := myLibSDL.so
MY_OpenGL_LIB := myLibOpenGL.so


# all: sdl2 sdl2_image sdl2_ttf sdl2_mixer sfml $(TARGET)
all: $(TARGET)
	# ./$(TARGET)

$(MY_SFML_LIB):
	@echo "\x1b[1mBuilding $(MY_SFML_LIB)...\x1b[0m"
	$(CC) -shared -fPIC $(HEADERS) \
		-o $(MY_SFML_LIB) $(OBJS) \
		$(SDL2_LINK) \
		$(SDL2_IMAGE_LINK) \
		$(SDL2_TTF_LINK) \
		$(SDL2_MIXER_LINK) \
		$(SFML_LINK) \
		$(GLEW_LINK) \
		-framework OpenGL -g -fsanitize=address

$(MY_SDL_LIB):
	@echo "\x1b[1mBuilding $(MY_SDL_LIB)...\x1b[0m"
	$(CC) -shared -fPIC $(HEADERS) \
		-o $(MY_SDL_LIB) $(OBJS) \
		$(SDL2_LINK) \
		$(SDL2_IMAGE_LINK) \
		$(SDL2_TTF_LINK) \
		$(SDL2_MIXER_LINK) \
		$(SFML_LINK) \
		$(GLEW_LINK) \
		-framework OpenGL -g -fsanitize=address

$(MY_OpenGL_LIB):
	@echo "\x1b[1mBuilding $(MY_OpenGL_LIB)...\x1b[0m"
	$(CC) -shared -fPIC $(HEADERS) \
		-o $(MY_OpenGL_LIB) $(OBJS) \
		$(SDL2_LINK) \
		$(SDL2_IMAGE_LINK) \
		$(SDL2_TTF_LINK) \
		$(SDL2_MIXER_LINK) \
		$(SFML_LINK) \
		$(GLEW_LINK) \
		-framework OpenGL -g -fsanitize=address

$(OBJDIR)%.o: $(SRCSDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $(HEADERS) $< -o $@

$(TARGET): $(OBJS) $(MY_SFML_LIB) $(MY_SDL_LIB) $(MY_OpenGL_LIB)
	@echo "\x1b[1mBuilding $(TARGET)...\x1b[0m"
	$(CC) -o $(TARGET) $(OBJS) \
		$(MY_SFML_LIB) $(MY_SDL_LIB) $(MY_OpenGL_LIB) \
		$(SDL2_LINK) \
		$(SDL2_IMAGE_LINK) \
		$(SDL2_TTF_LINK) \
		$(SDL2_MIXER_LINK) \
		$(SFML_LINK) \
		$(GLEW_LINK) \
		-framework OpenGL -g -fsanitize=address
	@echo "\x1b[1mBuild finished!!\x1b[0m"

clean:
	@echo "\x1b[1mCleaning...\x1b[0m"
	/bin/rm -rf $(OBJDIR)
	@echo

fclean: clean
	@echo "\x1b[1mFcleaning...\x1b[0m"
	/bin/rm -f $(TARGET) $(MY_SFML_LIB) $(MY_SDL_LIB) $(MY_OpenGL_LIB)
	@echo

re: fclean all

.PHONY: all clean fclean re sdl2 sdl2_image sdl2_ttf sdl2_mixer sfml
