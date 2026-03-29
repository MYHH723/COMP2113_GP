# Makefile for COMP2113_GP Roguelike Game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
DEBUGFLAGS = -std=c++11 -Wall -Wextra -g
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
DATA_DIR = data

# File extensions
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/game

# Targets
.PHONY: all build clean run debug help

# Default target
all: $(EXECUTABLE)

# Build the executable
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful! Executable: $@"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create directories if they don't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DATA_DIR):
	@mkdir -p $(DATA_DIR)

# Debug build
debug: CXXFLAGS = $(DEBUGFLAGS)
debug: clean $(EXECUTABLE)
	@echo "Debug build complete!"

# Run the game
run: $(EXECUTABLE) | $(DATA_DIR)
	./$(EXECUTABLE)

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Clean all including data
distclean: clean
	@rm -rf $(DATA_DIR)/*.json $(DATA_DIR)/*.log
	@echo "Distclean complete!"

# Display help
help:
	@echo "COMP2113_GP Makefile - Available targets:"
	@echo "  all        - Build the game (default)"
	@echo "  build      - Same as 'all'"
	@echo "  debug      - Build with debug symbols"
	@echo "  run        - Run the compiled game"
	@echo "  clean      - Remove build artifacts"
	@echo "  distclean  - Remove build artifacts and game data"
	@echo "  help       - Display this help message"

# Build alias
build: $(EXECUTABLE)
