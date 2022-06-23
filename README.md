# Rosary
Rosary is a library of standalone OpenGL applications (aka scenes) meant to serve as a playground and a cookbook of recipes at the same time. All these demonstrative scenes or applications were built over a number of years. Please refer to the *Scenes* section below for a detailed description of the individual scenes.

### Motivations and Design
All Rosary scenes are designed to run independent of each other. The system design choices and the coding principles employed for each scene may vary depending on the complexity of the graphics features in the scene. A scene may demonstrate the use of graphics API features, graphics algorithms or both. The elegance of the code design of the newer scenes tend to be better and robust as they attempt to present complex scenarios than the previous simpler scenes. However, there are exceptions to this pattern.

The third party dependencies are compiled only once and linked against the scenes, as required.

### Dependencies
The thirdparty libraries can be found as submodules in the /thirdparty folder. To check out Rosary together with all dependencies, be sure to use the `--recursive` flag when cloning the repository, i.e.

```
git clone --recursive https://github.com/ganeshbelgur/rosary.git
```

The following dependecies will be cloned along with the scenes,

- Glad (v0.1.36 @ 1ecd457) - OpenGL Extension Loader
- GLFW (v3.3.7 @ 45ce5dd) - Window and Context
- GLM (v0.9.9.8 @ bf71a83) - Math library
- Assimp (v5.2.4 @ c8dafe0) - Scenegraph serialization and deserialization
- STB (master) - Image deserialization

### Building Dependencies
Run the bash script to build all the dependecies. Building dependecies may require cmake, make, etc to be installed.

```
./build_dependencies.sh
```

### Building Scenes
We use the GNU make to build all the scenes in one go.
At the moment, it is not possible to build just a specific scene module. However, we welcome pull requests implementing this extension to the Makefile. Please refer the details of the extension required in the comments section of the Makefile.

We build all the objects and executables for each scene in the scenes folder into a separate folder under `$(TOP_LEVEL_BUILD_DIR)/` or `bin` folder by default.
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
NOTE: Currently this deletes even the thirdparty dependecies which is not right. It will be eventually fixed. Use the `build_dependencies.sh` again for now to rebuild all dependencies.
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

#### 11. shadow-mapping
Demonstartes the rendering of shadow maps using an additional framebuffer
