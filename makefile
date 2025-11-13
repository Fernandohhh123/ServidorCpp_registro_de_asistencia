CXX := g++

# Compilation flags
CXXFLAGS := -std=c++17 -Wall -Iinclude

# Linker flags (IMPORTANT: put libraries here)
LDFLAGS := -lmysqlcppconn -lpthread

# Source and object files
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=%.o)
TARGET := servidor

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rule
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

