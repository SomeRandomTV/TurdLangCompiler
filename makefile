CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/Compiler

# Default build
all: directories $(TARGET)

# Create necessary directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/Lexer
	mkdir -p $(OBJ_DIR)/SynParser

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories