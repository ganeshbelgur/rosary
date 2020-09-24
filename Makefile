# Variables required during compilation
CXX = g++
CFLAGS = -Wall
LDFLAGS = -lglfw -lGLEW -lGL -lX11 -lpthread -ldl
INCLUDE_PATH = ./thirdparty/include
LIBRARY_PATH = ./thirdparty/lib
TOP_LEVEL_SOURCE_DIR = scenes
TOP_LEVEL_BUILD_DIR = bin

# Find all the source directories
SOURCE_DIRS = $(wildcard ./$(TOP_LEVEL_SOURCE_DIR)/*)
# Build all the build directories to be generated
MODULE_DIRS = $(subst /$(TOP_LEVEL_SOURCE_DIR)/,/$(TOP_LEVEL_BUILD_DIR)/,$(SOURCE_DIRS))
# Build all the scene module names to be generated
MODULES = $(foreach directory,$(MODULE_DIRS),$(directory)/$(word 3,$(subst /, ,$(directory)))-executable)

# Build+Run logic:
# Build all the objects and executables for each 
# scene in the scenes folder into a separate folder 
# under '$(TOP_LEVEL_BUILD_DIR)/'. Choose and run 
# a final executable using the `make run` command. 

# Possible extentions needed to this makefile in the future:
# "1. Ability to build only a specific scene module."
# Currently, it will build all scene modules found 
# under ./$(TOP_LEVEL_SOURCE_DIR) when make runs.
# It is important to note that adding this feature will 
# have an effect on how the build directories are generated.
# All the build directories cannot be built up front if we
# allow only a specific module to be built at a time. 
# The build directories will need to be built as and when
# the object files for the specific scene are ready 
# (probably with Make's order-only prerequisite directive)
#
# "2. Ability to run multiple scene modules at a time."
# Currently, only a specific scene module specified by
# $(SCENE) can be run using the `make run` command.

.PHONY: all clean run
all: $(MODULE_DIRS) $(MODULES)

# Generates all the necessary build folders
$(MODULE_DIRS):
	mkdir -p $@

# Function to construct a module's rule
define GENERATE_RULE
# Using eval here because direct assignment 
# caused strange issues where the variables 
# where turning out to be empty strings
$(eval SOURCES=$(wildcard ./$(TOP_LEVEL_SOURCE_DIR)/$1/*.cpp))
$(eval OBJECTS=$(patsubst %,./$(TOP_LEVEL_BUILD_DIR)/$1/%, $(notdir $(SOURCES:.cpp=.o))))
$(eval SOURCE_DIR=./$(TOP_LEVEL_SOURCE_DIR)/$1)
$(eval OBJECT_DIR=./$(TOP_LEVEL_BUILD_DIR)/$1)

# Important to use double '$' for automatic 
# variables inside the make function.
# The '$$' delays the evaluation of the automatic variables 
# until the evaluation of the rules themselves. 
./bin/$1/$1-executable: $(OBJECTS)
	$(CXX) -L$(LIBRARY_PATH) -o $$@ $$^ $(LDFLAGS)

$(OBJECTS): $(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(INCLUDE_PATH) -L$(LIBRARY_PATH) -c $$< $(LDFLAGS) -o $$@
endef

# Constructing module rule for all the modules
$(foreach module,$(MODULES),$(eval $(call GENERATE_RULE,$(word 3,$(subst /, ,$(module))))))

# Use `make clean` to remove the bin folder
clean:
	@echo "Removing the build folder..."
	rm -rf $(TOP_LEVEL_BUILD_DIR)

# Use `make run` to execute your example program
# by passing in program name to $(SCENE)
run:
	LD_LIBRARY_PATH=./thirdparty/lib ./bin/$(SCENE)/$(SCENE)-executable
