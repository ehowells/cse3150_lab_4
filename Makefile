# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -g

# Target executable
TARGET := solution
SRC := solution.cpp
OBJ := $(SRC:.cpp=.o)

.PHONY: all clean test

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source into object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Python tests
test: $(TARGET)
	pytest test_game.py

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
