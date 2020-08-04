# Variables required during compilation
CXX = g++
CFLAGS = -Wall
LDFLAGS = -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl
INCLUDE_PATH = ./thirdparty/include
LIBRARY_PATH = ./thirdparty/lib
MODULE_NAME = opengl-scene
BUILD_DIR = bin

# Searching for source files and generating object file names
SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %,$(BUILD_DIR)/%, $(notdir $(SOURCE_FILES:.cpp=.o)))

# Beginning build process
$(BUILD_DIR)/$(MODULE_NAME): $(OBJECT_FILES)
	$(CXX) -L$(LIBRARY_PATH) -o $@ $^ $(LDFLAGS)

# Using the vpath directive to fetch the right
# source file to build the right object file.
vpath %.cpp $(dir $(SOURCE_FILES))

# Since the timestamps on directories change whenever a file is
# added, removed, or renamed, we certainly don’t want to rebuild all
# the targets whenever the build directory’s timestamp changes.
# One way to manage this is with order-only prerequisites:
# make the directory an order-only prerequisite on all the targets
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -I$(INCLUDE_PATH) -L$(LIBRARY_PATH) -c $< $(LDFLAGS) -o $@

# This is an order-only prerequisite for
# the creation of the build directory before
# the generation of the object files
$(BUILD_DIR):
	@mkdir -p "$(BUILD_DIR)"

# Use `make help` to find all the source files in the project
.PHONY: help
help:
	@echo "SOURCE_FILES: $(SOURCE_FILES)"

# Use `make clean` to remove the bin folder
.PHONY: clean
clean:
	@echo "Removing the build folder..."
	rm -rf bin

# Use `make run` to execute your program
.PHONY: run
run:
	LD_LIBRARY_PATH=./thirdparty/lib ./bin/opengl-scene