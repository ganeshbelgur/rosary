# Rosary
Rosary is a library of standalone OpenGL applications meant to serve as a playground and a cookbook of recipes at the same time. The library of scenes here were built over a number of years. Please refer to the *Scenes* section below for a detailed description of the Rosary scenes.

### Motivations and Design
Most OpenGL code samples do not record the system design choices made over time as examples get complicated. However, Rosary scenes do show the evolution of such choices by just leaving the older and simpler scenes with their naive system design choices. This caveat was deliberately left to take shape over time as the catalogue of scenes grew. It showcases the ever evolving complexities in computer graphics algorithms and system design choices while using OpenGL. Naturally, the newer scenes appear to be more robust and cleaner than the older ones.

There are no cross referencing of entities across scenes: while a newer scene might duplicate the code and build upon the code from an older scene, each scene is meant to function in a standalone fashion even though it could blow up the codebase size a bit over time. Obviously, third party dependencies are an exception to this rule. We do not duplicate the third party dependencies for each scene. Further, one can even notice bugs in earlier scenes getting fixed over time even though we have tried really hard to not let potentially hazardous bugs from cascading through the scenes.

### Building Scenes
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
Renders a simple triangle with a basic vertex and fragment shader
The shader sources are specified in the program itself as strings.

#### 2. moving-triangle
Demonstrates use of GLM library to apply transforms on a simple triangle via a uniform variable

#### 3. simple-interpolation
Demonstrates interpolation of vertex color attribute in GLSL

#### 4. first-3d-object
Draws tetrahedron object using *Index Buffer Objects (IBOs/EBOs)* with depth testing enabled
We no longer work with just two-dimensional triangles.

#### 5. perspective-projection
Converts objects from local space to clip space with a perspective projection matrix applied.
However, the view matrix is still missing because there is no camera system in place.

#### 6. structured-scene
Demonstrates clean separation of window management, shader management and mesh generation by segregating those functionalities into their component classes to increase code reusability. For the first time, the shader sources are moved out into their respective files with `.glsl` extensions and loaded off the disk at run time.

#### 7. camera-system
Adds ability to move around the scene with a restricted camera system (*First Person Shooter*) that calculates yaw, pitch, movement from user inputs via callbacks to create a view matrix. The delta time factor allows the speed of the camera in the scene to be consistent across different machines.

#### 8. simple-texture-mapping
Demonstrates texture mapping by loading two different image textures off disk using the stb library. Further, texture parameters such as texture wrapping, texture filtering with handling of texture units and texture samplers inside shaders are also demonstrated.

#### 9. phong-lighting-and-shading
Demonstrates scene lighting with Phong lighting and shading models with interpolated normals. The lighting components are broken up into ambient lighting, diffuse lighting and specular lighting. Further, the scene demonstrates the use of a single directional light source with multiple point lights and spot lights (with feathered edges).

#### 10. loading-complex-models
Demonstrates the use of Open Assimp library to load two complex models. The first model is a X-wing spacecraft from Star Wars and the second model is a Black Hawk, a helicopter used for military operations.
