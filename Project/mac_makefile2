# Compiler
#CXX = clang++ 

#@author Isveydi Moreno
EMPTY = 
SPACE = $(EMPTY) $(EMPTY)

# Get SFML directory, ensuring it is formatted correctly in case its directory contains spaces
#SFML_DIR ?=
#S_DIR = $(subst $(SPACE),\$(SPACE),$(SFML_DIR))

#Isveydi's SFML_DIR
SFML_DIR=/usr/local/opt/sfml@2/

# -------[[ BUILD CONFIGURATION ]]-------
# Assume that we are building the debug build
DEBUG ?= TRUE #Changed to TRUE for debugging

# -------[[ USED DIRECTORIES ]]-------

# Add any additional include directories here
USER_INCLUDE_DIRS = -Iinclude -I$(SFML_DIR)/include

# Add any additional library directories
LIB_DIRS = -L$(SFML_DIR)/lib


# -------[[ USED LIBRARIES AND SOURCE FILES ]]-------

# Libraries used during linking
ifeq ($(DEBUG), TRUE)
	LIBS = -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-audio-s-d -lsfml-system-s-d -framework OpenGL
else
	LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -framework OpenGL
endif

# Add any additional source files to here
SRCS = Framework_Managers/AudioManager.cpp Framework_Managers/FontManager.cpp Framework_Managers/InputManager.cpp Framework_Managers/TextureManager.cpp Framework_Managers/SaveManager.cpp \
		Gamestate_Operators/Static/HitboxManager.cpp Gamestate_Operators/Static/LevelManager.cpp Gamestate_Operators/Static/MapManager.cpp Gamestate_Operators/Static/PlayerManager.cpp Gamestate_Operators/Static/ScoreManager.cpp \
		Gamestate_Operators/Instanced/PauseMenuOperator.cpp \
		Gamestate_Operators/Instanced/TitleScreenActorOperator.cpp \
		Gamestate_Operators/Instanced/EndingCutsceneActorOperator.cpp \
		Enemy.cpp EnemyManager.cpp  \
		BirdEnemy.cpp DogEnemy.cpp DogEnemy2.cpp RatEnemy.cpp \
		Player.cpp NPC.cpp\
		Gamestates/PlayMainGameState.cpp Gamestates/PauseGameState.cpp Gamestates/TitleScreenGameState.cpp Gamestates/EndingCutsceneGameState.cpp \
		Entry.cpp

# Prepend src/ to the source files and create list of object files
SRCS := $(addprefix src/, $(SRCS))
OBJS = $(SRCS:src/%.cpp=obj/%.o)


# -------[[ BUILD FLAGS ]]-------

ifeq ($(DEBUG), 1)
	CXX_FLAGS_BASE = -Wall -Wextra -Werror -Wno-delete-non-abstract-non-virtual-dtor -Wno-deprecated-declarations -g -Og -std=c++20 -DDEBUG
else
	CXX_FLAGS_BASE = -Wall -Wextra -Werror -Wno-delete-non-abstract-non-virtual-dtor -Wno-deprecated-declarations -O3 -std=c++20 -DNDEBUG
endif

CXX_FLAGS = $(CXX_FLAGS_BASE) \
$(LIB_DIRS) \
$(LIBS) \
$(USER_INCLUDE_DIRS)



# -------[[ TARGETS & RULES ]]-------

.DEFAULT_GOAL := all

all: $(OBJS) Project

Project: $(OBJS)
	@mkdir -p bin
	@echo "\nLinking final executable $@...\n"
	@$(CXX) $(OBJS) $(CXX_FLAGS) -o bin/$@
	@echo "\nInstalling assets to binary directory...\n"
	@cp -r assets bin
	@echo "\n\nDONE!!\n\n"

$(OBJS): obj/%.o: src/%.cpp
	@echo "\nBuilding translation unit for $<...\n"
	@mkdir -p $(dir $(OBJS))
	@$(CXX) $(CXX_FLAGS_BASE) $(USER_INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf obj
	rm -rf bin
