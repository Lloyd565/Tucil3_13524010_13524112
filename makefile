CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc/include

SRC_DIR = src/src
BIN_DIR = bin
TARGET = $(BIN_DIR)/tucil3

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)

-include $(DEPS)
