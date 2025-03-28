CXX = g++
CXXFLAGS = -Ilib -Wall -Wextra -O2
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj
BIN_DIR = bin

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
LIB_FILES = $(wildcard $(LIB_DIR)/*.cpp)
SOURCES = $(SRC_FILES) $(LIB_FILES)

# Object files
SRC_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LIB_OBJ = $(patsubst $(LIB_DIR)/%.cpp,$(OBJ_DIR)/lib_%.o,$(LIB_FILES))
OBJECTS = $(SRC_OBJ) $(LIB_OBJ)

# Target binary
TARGET = $(BIN_DIR)/pensieve

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile library files
$(OBJ_DIR)/lib_%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

# Generate dependencies
deps:
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > deps.mk
-include deps.mk

.PHONY: all clean run debug deps