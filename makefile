CXX := g++

# Compilation flags
CXXFLAGS := -std=c++17 -Wall -Iinclude

# Linker flags (IMPORTANT: put libraries here)
LDFLAGS := -lmysqlcppconn -lpthread

# Source and object files
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=obj/%.o)
TARGET := bin/cerrusv

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rule
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

