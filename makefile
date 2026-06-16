# Compiler
CXX := g++

# Dynamic compilation flags
CXXFLAGS := -std=c++17 -Wall -Wextra

# Dynaimc linker flags (IMPORTANT: put libraries here)
LDFLAGS := -lmysqlcppconn -lpthread

# Source and object files
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=obj/%.o)
TARGET := bin/cerrusv

# Default target
all: $(TARGET)

# En caso de querer compilar estatico
static: CXXFLAGS := -std=c++17 -Wall -Wextra -DSTATIC_CONCPP
static: LDFLAGS := /usr/lib/x86_64-linux-gnu/libmysqlcppconn-static.a \
					-lmysqlclient \
					-lssl -lcrypto -lpthread -ldl
static: $(TARGET)

# Linking rule
$(TARGET): $(OBJ)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rule
obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean static

# Clean
clean:
	rm -f $(OBJ) $(TARGET)
