# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -g

# Target executable
TARGET := solution
SRC := solution.cpp

.PHONY: all clean test run

# Default target
all: $(TARGET)

# Build executable directly from source
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Run the C++ game
run: $(TARGET)
	./$(TARGET)

# Run Python tests
test: $(TARGET)
	pytest test_game.py

# Clean build artifacts
clean:
	rm -f $(TARGET)
