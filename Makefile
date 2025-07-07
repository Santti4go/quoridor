# Makefile for Quoridor Game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
# For GUI version
# SFML linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
# For GUI version 
# SRCS = main.cpp GameModel.cpp ConsoleView.cpp GameController.cpp
SRCS = main.cpp GameModel.cpp GuiView.cpp GuiController.cpp
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
	rm -f $(OBJS)

.PHONY: all clean
