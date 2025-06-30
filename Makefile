# Makefile for Quoridor Game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
LDFLAGS =

# Source files
SRCS = main.cpp GameModel.cpp ConsoleView.cpp GameController.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = quoridor

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
