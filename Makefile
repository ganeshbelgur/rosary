# Variables required during compilation
CXX = g++
CFLAGS = -Wall
LDFLAGS = -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl
INCLUDE_PATH = ./thirdparty/include
LIBRARY_PATH = ./thirdparty/lib
BUILD_DIR = bin

DIRECTORIES = $(wildcard ./scenes/*)
MODULE_DIRECTORIES = $(subst /scenes/,/bin/,$(DIRECTORIES))
MODULES = $(foreach directory,$(MODULE_DIRECTORIES),$(directory)/$(word 3,$(subst /, ,$(directory)))-executable)

.PHONY: all clean run
all: $(MODULE_DIRECTORIES) $(MODULES)

# Generates all the necessary build folders
$(MODULE_DIRECTORIES):
	mkdir -p $@

# Function to construct a module's rule
define GENERATE_RULE
# Using eval here because direct assignment 
# caused strange issues where the variables 
# where turning out to be empty strings
$(eval SOURCE_FILES=$(wildcard ./scenes/$1/*.cpp))
$(eval OBJECT_FILES=$(patsubst %,./bin/$1/%, $(notdir $(SOURCE_FILES:.cpp=.o))))
$(eval SOURCE_DIRECTORY=./scenes/$1)
$(eval OBJECT_DIRECTORY=./bin/$1)

# Important to double '$' for 
# automatic variables inside a make function 
./bin/$1/$1-executable: $(OBJECT_FILES)
	$(CXX) -L$(LIBRARY_PATH) -o $$@ $$^ $(LDFLAGS)

$(OBJECT_FILES): $(OBJECT_DIRECTORY)/%.o : $(SOURCE_DIRECTORY)/%.cpp
	$(CXX) $(CFLAGS) -I$(INCLUDE_PATH) -L$(LIBRARY_PATH) -c $$< $(LDFLAGS) -o $$@
endef

# Constructing module rules for everything in $(MODULES)
$(foreach module,$(MODULES),$(eval $(call GENERATE_RULE,$(word 3,$(subst /, ,$(module))))))

# Use `make clean` to remove the bin folder
clean:
	@echo "Removing the build folder..."
	rm -rf $(BUILD_DIR)

# Use `make run` to execute your program 
# by passing in program name to $(PROGRAM)
run:
	LD_LIBRARY_PATH=./thirdparty/lib ./bin/$(PROGRAM)/$(PROGRAM)-executable
