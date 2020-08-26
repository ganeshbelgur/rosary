# Rosary
Rosary is collection of standalone OpenGL applications meant to serve as a playground and a cookbook of recipes at the same time. The scenes here were added to the collection over a number of years. Please refer to the Scenes section below for a detailed description of the Rosary scenes.

### Building scenes
We use the GNU make to build all the scenes in one go.
At the moment, it is not possible to build just a specific scene module. However, we welcome pull requests implementing this extension to the Makefile. Please refer the details of the extension required in the comments section of the Makefile.

We build all the objects and executables for each scene in the scenes folder into a separate folder under `$(TOP_LEVEL_BUILD_DIR)/`.
```
make -j8
```

### Running a specific scene
To run a scene scene use the `$SCENE` variable to specify the scene name.
```
make run SCENE="simple-triangle"
```

### Cleaning the build
The clean process just recursively removes the `$(TOP_LEVEL_BUILD_DIR)` directory where all the objects and executables were placed during the build process.
```
make clean
```

### Scenes
The following is the description of the recipe scenes found under `./scenes` folder.

#### 1. simple-triangle
Renders a simple triangle with a basic vertex and fragment shader.
The shader sources are specified in the program itself as strings.

#### 2. moving-triangle
Demonstrates use of GLM library to apply transforms on a simple triangle.

#### 3. simple-interpolation
Demonstrates interpolation of vertex color attribute in GLSL

#### 4. first-3d-object
Draws tetrahedron object using *Index Buffer Objects (IBOs/EBOs)* with depth testing enabled

#### 5. perspective-projection
Converts objects from local space to clip space with a perspective projection matrix applied.
However, the view matrix is still missing because there is no camera system in place.

#### 6. structured-scene
Demonstrates clean separation of window management, shaders and meshes into their component classes to increase code reusability. For the first time, the shader sources are moved out into their respective files with `.glsl` extensions.
