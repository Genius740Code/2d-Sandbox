CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
SFML_INCLUDE = -I./SFML/include
SFML_LIB_DIR = ./SFML/build/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Detect Windows platform and adjust commands accordingly
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist $(subst /,\,$1) mkdir $(subst /,\,$1)
    RM = if exist $(subst /,\,$1) rmdir /s /q $(subst /,\,$1)
    COPY = if exist $(subst /,\,$1) copy $(subst /,\,$1) $(subst /,\,$2)
    PATHSEP = \\
    EXE = .exe
else
    MKDIR = mkdir -p $1
    RM = rm -rf $1
    COPY = cp -f $1 $2
    PATHSEP = /
    EXE =
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

MAIN = $(BIN_DIR)/terrain_generator$(EXE)

SRCS = $(SRC_DIR)/main.cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: directories $(MAIN)

directories:
	$(call MKDIR,$(OBJ_DIR))
	$(call MKDIR,$(BIN_DIR))

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(MAIN) -L$(SFML_LIB_DIR) $(SFML_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) -c $< -o $@

# Try alternative paths for SFML libraries if the default path doesn't work
check_paths:
	@echo "Checking SFML paths..."
	@if exist SFML\lib (echo "Found SFML\lib") else (echo "SFML\lib not found")
	@if exist SFML\build\lib (echo "Found SFML\build\lib") else (echo "SFML\build\lib not found")
	@if exist SFML\bin (echo "Found SFML\bin") else (echo "SFML\bin not found")
	@if exist SFML\build\bin (echo "Found SFML\build\bin") else (echo "SFML\build\bin not found")

# Windows-specific DLL copying
copy_dlls:
	$(call MKDIR,$(BIN_DIR))
	@echo "Trying to copy DLLs from multiple possible locations..."
	-$(call COPY,SFML$(PATHSEP)bin$(PATHSEP)*.dll,$(BIN_DIR)$(PATHSEP))
	-$(call COPY,SFML$(PATHSEP)build$(PATHSEP)bin$(PATHSEP)*.dll,$(BIN_DIR)$(PATHSEP))
	-$(call COPY,SFML$(PATHSEP)extlibs$(PATHSEP)bin$(PATHSEP)*.dll,$(BIN_DIR)$(PATHSEP))

run: $(MAIN)
	@echo "Running $(MAIN)..."
	@$(MAIN)

clean:
	$(call RM,$(OBJ_DIR))
	$(call RM,$(BIN_DIR))

.PHONY: all clean run directories copy_dlls check_paths 