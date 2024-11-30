# File: makefile

# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Source and Object Files
SRC = src/main.cpp \
      src/init.cpp \
      src/hash_object.cpp \
      src/sha1.cpp \
      src/cat_file.cpp \
      src/write_tree.cpp \
      src/ls_tree.cpp \
      src/add.cpp \
      src/commit.cpp \
      src/log.cpp

OBJ = $(SRC:.cpp=.o)

# Executable Name
TARGET = mygit

# Default Target
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile .cpp Files to .o Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Up Compiled Files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony Targets
.PHONY: all clean
