# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall

# Target executable
TARGET = solution

# Source files
SOURCES = solution.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Run tests
test: $(TARGET)
	pytest test_game.py

# Clean up compiled files
clean:
	rm -f $(TARGET)

# Phony targets (not actual files)
.PHONY: all test clean