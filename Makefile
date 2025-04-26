# Windows Makefile for 2D Minecraft
# Run with: mingw32-make

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17 
LDFLAGS = -lgdi32 -luser32 -lgdiplus

# Source and build directories
SRCDIR = src
OBJDIR = obj

# Executable name
EXECUTABLE = main
TEST_EXECUTABLE = test_perlin

# Source files
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/engine/PerlinNoise.cpp \
          $(SRCDIR)/engine/TextureManager.cpp \
          $(SRCDIR)/game/World.cpp \
          $(SRCDIR)/game/Game.cpp

# Test source files
TEST_SOURCES = $(SRCDIR)/TestPerlinNoise.cpp \
               $(SRCDIR)/engine/PerlinNoise.cpp

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# OS detection - assumes Windows
RM = del /Q /S
MKDIR = mkdir
EXE_EXT = .exe

# Create output directories
$(shell if not exist $(OBJDIR) mkdir $(OBJDIR))
$(shell if not exist $(OBJDIR)\engine mkdir $(OBJDIR)\engine)
$(shell if not exist $(OBJDIR)\game mkdir $(OBJDIR)\game)

.PHONY: all clean run test test_run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@$(EXE_EXT) $(LDFLAGS)
	@echo "Build complete: $@"
	@echo "Run with: ./$(EXECUTABLE)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test targets
test: $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@$(EXE_EXT) $(LDFLAGS)
	@echo "Test build complete: $@"
	@echo "Run with: ./$(TEST_EXECUTABLE)"

test_run: test
	./$(TEST_EXECUTABLE)$(EXE_EXT)

clean:
	$(RM) $(OBJDIR)
	$(RM) $(EXECUTABLE)$(EXE_EXT)
	$(RM) $(TEST_EXECUTABLE)$(EXE_EXT)

run: all
	./$(EXECUTABLE)$(EXE_EXT)

# Quick build (legacy)
quick:
	g++ -Wall -Wextra -O3 -std=c++17 main.cpp -o main.exe -lgdi32 -luser32
