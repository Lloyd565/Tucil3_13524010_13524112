CXX = g++
GUI_CXX = $(CXX)
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc/include
DEPFLAGS = -MMD -MP
GUI_DEPFLAGS = $(DEPFLAGS)

SRC_DIR = src/src
BIN_DIR = bin
CLI_TARGET = $(BIN_DIR)/tucil3-cli
GUI_TARGET = $(BIN_DIR)/tucil3-gui
CLI_EXE_EXT =
GUI_EXE_EXT =

ifeq ($(OS),Windows_NT)
	RAYLIB_ROOT ?= D:/Language/raylib/raylib
	RAYLIB_INC = -I$(RAYLIB_ROOT)/src
	RAYLIB_LIB = -L$(RAYLIB_ROOT)/src -lraylib -lopengl32 -lgdi32 -lwinmm
	CLI_EXE_EXT = .exe
	GUI_EXE_EXT = .exe
else ifneq ($(wildcard /mnt/d/Language/raylib/w64devkit/bin/g++.exe),)
	GUI_CXX = bash tools/w64-g++.sh
	RAYLIB_ROOT ?= D:/Language/raylib/raylib
	RAYLIB_INC = -I$(RAYLIB_ROOT)/src
	RAYLIB_LIB = -L$(RAYLIB_ROOT)/src -lraylib -lopengl32 -lgdi32 -lwinmm
	GUI_EXE_EXT = .exe
	GUI_DEPFLAGS =
else
	RAYLIB_ROOT ?=
	ifeq ($(shell pkg-config --exists raylib && echo yes),yes)
		RAYLIB_INC = $(shell pkg-config --cflags raylib)
		RAYLIB_LIB = $(shell pkg-config --libs raylib)
	else ifneq ($(strip $(RAYLIB_ROOT)),)
		RAYLIB_INC = -I$(RAYLIB_ROOT)/src -I$(RAYLIB_ROOT)/include
		RAYLIB_LIB = -L$(RAYLIB_ROOT)/src -L$(RAYLIB_ROOT)/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	else
		RAYLIB_INC =
		RAYLIB_LIB = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	endif
endif

COMMON_SRCS = $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/guimain.cpp $(SRC_DIR)/frontend/%,$(shell find $(SRC_DIR) -name '*.cpp'))
CLI_SRCS = $(COMMON_SRCS) $(SRC_DIR)/main.cpp
GUI_SRCS = $(COMMON_SRCS) $(shell find $(SRC_DIR)/frontend -name '*.cpp') $(SRC_DIR)/guimain.cpp

CLI_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/cli/%.o,$(CLI_SRCS))
GUI_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/gui/%.o,$(GUI_SRCS))
DEPS = $(CLI_OBJS:.o=.d) $(if $(strip $(GUI_DEPFLAGS)),$(GUI_OBJS:.o=.d))

.PHONY: all choose cli gui run run-cli run-gui clean

all: choose

choose:
	@printf "Choose app mode (cli/gui): "; \
	read mode; \
	case "$$mode" in \
		cli|CLI|c|C) $(MAKE) run-cli ;; \
		gui|GUI|g|G) $(MAKE) run-gui ;; \
		*) echo "Invalid mode. Use cli or gui."; exit 1 ;; \
	esac

cli: $(CLI_TARGET)$(CLI_EXE_EXT)

gui: $(GUI_TARGET)$(GUI_EXE_EXT)

$(CLI_TARGET)$(CLI_EXE_EXT): $(CLI_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GUI_TARGET)$(GUI_EXE_EXT): $(GUI_OBJS)
	@mkdir -p $(dir $@)
	$(GUI_CXX) $(CXXFLAGS) $^ -o $@ $(RAYLIB_LIB)

$(BIN_DIR)/cli/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BIN_DIR)/gui/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GUI_CXX) $(CXXFLAGS) $(RAYLIB_INC) $(GUI_DEPFLAGS) -c $< -o $@

run: choose

run-cli: cli
	./$(CLI_TARGET)$(CLI_EXE_EXT)

run-gui: gui
	./$(GUI_TARGET)$(GUI_EXE_EXT)

clean:
	rm -rf $(BIN_DIR)

-include $(DEPS)
