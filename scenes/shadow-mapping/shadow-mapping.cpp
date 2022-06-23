//
//  Rosary source code is Copyright(c) 2016-2020 Ganesh Belgur
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//  - Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  - Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window-manager.h"
#include "mesh.h"
#include "shader-manager.h"
#include "camera.h"
#include "directional-light.h"
#include "material.h"
#include "model.h"

// Scene data
WindowManager window;
std::vector<Mesh*> meshes;
ShaderManager directLightShadowMapShader;
std::vector<ShaderManager> shaderManagers;
Camera camera;
DirectionalLight directionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Material shinyMaterial;
Material dullMaterial;
Model xWing;
Model blackhawk;

// Path to the shader files relative to Rosary's Makefile
static const char* vertexShaderPath = "./scenes/shadow-mapping/shaders/vertex.glsl";
static const char* fragmentShaderPath = "./scenes/shadow-mapping/shaders/fragment.glsl";

// Special shader uniform locations
GLuint uniformModelLocation = 0;

// Initialise spot lights
unsigned int numberOfSpotLights = 0;

// Initialise point lights
unsigned int numberOfPointLights = 0;

// Blackhawk Rotation
float blackHawkAngle = 0.0f;

// Average the normals at the vertex positions
// for all the triangular surfaces, no interpolation
// performed yet. Interpolation occurs in the shaders!
void calculateNormalsForPhongShading(
    GLfloat *vertices,
    unsigned int numOfVertices,
    unsigned int *indices, 
    unsigned int numOfIndices,
    unsigned int perVertexDataLength,
    unsigned int normalDataOffset)
{
    for(size_t i = 0; i < numOfIndices; i += 3)
    {
        // Find the starting location in the data array for 
        // the vertices making up the given triangluar surface
        unsigned int vertex0 = indices[i] * perVertexDataLength;
        unsigned int vertex1 = indices[i+1] * perVertexDataLength;
        unsigned int vertex2 = indices[i+2] * perVertexDataLength;

        // Calculate the vectors required to generate 
        // the normal vector for the particular trianglar face
        glm::vec3 sideVector1(
            vertices[vertex1] - vertices[vertex0],
            vertices[vertex1 + 1] - vertices[vertex0 + 1],
            vertices[vertex1 + 2] - vertices[vertex0 + 2]);

        glm::vec3 sideVector2(
            vertices[vertex2] - vertices[vertex0],
            vertices[vertex2 + 1] - vertices[vertex0 + 1],
            vertices[vertex2 + 2] - vertices[vertex0 + 2]    
        );

        // Calculate the normal vector
        glm::vec3 normal = glm::cross(sideVector1, sideVector2);

        // Calculate the normalised version of the normal
        normal = glm::normalize(normal);

        // Find the starting location in the daya array
        // to populate the normal information
        // for each vertex in the given triangular faces
        vertex0 += normalDataOffset;
        vertex1 += normalDataOffset;
        vertex2 += normalDataOffset;
        
        // Add the normal contribution to the 
        // vertex daya array
        vertices[vertex0] += normal.x;
        vertices[vertex0 + 1] += normal.y;
        vertices[vertex0 + 2] += normal.z;

        vertices[vertex1] += normal.x;
        vertices[vertex1 + 1] += normal.y;
        vertices[vertex1 + 2] += normal.z;

        vertices[vertex2] += normal.x;
        vertices[vertex2 + 1] += normal.y;
        vertices[vertex2 + 2] += normal.z;
    }

    for(size_t i = 0; i < numOfVertices/perVertexDataLength; i++)
    {
        // Get the starting location of the normal data in the vertex data array
        unsigned int normalOffset = i * perVertexDataLength + normalDataOffset;

        // Procure the normal vector
        glm::vec3 normal(
            vertices[normalOffset],
            vertices[normalOffset + 1],
            vertices[normalOffset + 2]);
        glm::vec3 normalizedNormal(1.0f, 1.0f, 1.0f);

        // Normalize the normal vector
        normalizedNormal = glm::normalize(normal);

        vertices[normalOffset] = normal.x;
        vertices[normalOffset + 1] = normal.y;
        vertices[normalOffset + 2] = normal.z;
    }
}

void CreateMeshes()
{
    // Specifying the triangular surfaces 
    // using indices (of vertices array) for 
    // index drawing
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // Vertex positions for the tetrahedron
    GLfloat vertices[] = {
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,    0.5f, 0.0f,    0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,    1.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,    0.5f, 1.0f,    0.0f, 0.0f, 0.0f
    };
    
    // Calculate the averaged normals required
    // for Phong shading that will be later
    // interpolated across all the triangular
    // surfaces of the tetrahedron
    calculateNormalsForPhongShading(
        vertices,
        32,
        indices,
        12,
        8,
        5
    );

    // Specifying the triangular surfaces
    // for the floor
    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    // Vertex positions for the floor
    GLfloat floorVertices[] = {
        -10.0f,  0.0f, -10.0f,    0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
         10.0f,  0.0f, -10.0f,   10.0f,  0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f,  0.0f,  10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
         10.0f,  0.0f,  10.0f,   10.0f, 10.0f,    0.0f, -1.0f, 0.0f
    };

    // Generate the tetrahedron meshes
    Mesh *firstTetrahedron = new Mesh();
    firstTetrahedron->createMesh(vertices, indices, 32, 12);
    meshes.push_back(firstTetrahedron);

    Mesh *secondTetrahedron = new Mesh();
    secondTetrahedron->createMesh(vertices, indices, 32, 12);
    meshes.push_back(secondTetrahedron);

    Mesh *floor = new Mesh();
    floor->createMesh(floorVertices, floorIndices, 32, 6);
    meshes.push_back(floor);
}

void CreateShaderPrograms()
{
    // Load the shaders from file and create shader program object
    ShaderManager *shaderManager = new ShaderManager();
    shaderManager->createShaderProgramFromFiles(vertexShaderPath, fragmentShaderPath);
    shaderManagers.push_back(*shaderManager);

    // Creating a separate shader to handle the direct light shadow map
    directLightShadowMapShader = ShaderManager();
    directLightShadowMapShader.createShaderProgramFromFiles(
        "./scenes/shadow-mapping/shaders/directional-light-shadow-map-vertex.glsl",
        "./scenes/shadow-mapping/shaders/directional-light-shadow-map-fragment.glsl");
}

void RenderScene()
{
    // Begin rendering the individual models
    // Generate the model matrix for the first tetrahedron
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

    // Bind the matrix data to the uniform variable in the shader
    // Arguments: location, number of matrices, 
        // transpose matrices?, pointer to the matrix/matrices
    glUniformMatrix4fv(
        uniformModelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    // Using the brick texture to render the first tetrahedron
    brickTexture.useTexture();

    // Add in the shiny specular material properties
    // for the first tetrahedron
    shinyMaterial.useMaterial(
        shaderManagers[0].getUniformSpecularIntensityLocation(),
        shaderManagers[0].getUniformShininessLocation()
    );

    // Render the first tetrahedron
    meshes[0]->renderMesh();

    // Setting up the model matrix for the second tetrahedron
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));

    // Setting the new model matrix into the shader
    glUniformMatrix4fv(
        uniformModelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    // Using the dirt texture to render the second tetrahedron
    dirtTexture.useTexture();

    // Add in the dull material properties
    // for the second tetrahedron
    dullMaterial.useMaterial(
        shaderManagers[0].getUniformSpecularIntensityLocation(),
        shaderManagers[0].getUniformShininessLocation()
    );
    
    // Render the second tetrahedron
    meshes[1]->renderMesh();

    // Setting up the model matrix for the floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));

    // Setting the new model matrix into the shader
    glUniformMatrix4fv(
        uniformModelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    // Using the plain texture to render the floor
    dirtTexture.useTexture();

    // Add in the dull material properties
    // for the floor
    shinyMaterial.useMaterial(
        shaderManagers[0].getUniformSpecularIntensityLocation(),
        shaderManagers[0].getUniformShininessLocation()
    );
    
    // Render the floor
    meshes[2]->renderMesh();

    // Setting up the model matrix for the xwing model
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
    model = glm::scale(model, glm::vec3(0.006f, 0.006, 0.006f));

    // Setting the new model matrix into the shader
    glUniformMatrix4fv(
        uniformModelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    // Add in the dull material properties
    // for the xwing
    shinyMaterial.useMaterial(
        shaderManagers[0].getUniformSpecularIntensityLocation(),
        shaderManagers[0].getUniformShininessLocation()
    );
    
    // Render the xwing model
    xWing.renderModel();

    blackHawkAngle += 1.0f;
    if (blackHawkAngle > 360.0f)
    {
        blackHawkAngle = 0.1f;
    }

    // Setting up the model matrix for the black hawk model
    model = glm::mat4(1.0f);
    model = glm::rotate(model, -blackHawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-8.0f, 0.01f, 0.0f));
    model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // Setting the new model matrix into the shader
    glUniformMatrix4fv(
        uniformModelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    // Add in the dull material properties
    // for the black hawk
    shinyMaterial.useMaterial(
        shaderManagers[0].getUniformSpecularIntensityLocation(),
        shaderManagers[0].getUniformShininessLocation()
    );
    
    // Render the black hawk model
    blackhawk.renderModel();
}

void RenderDirectLightShadowMap(DirectionalLight *light)
{
    directLightShadowMapShader.useShader();
    DirectionalLightShadowMap* shadowMap = light->getShadowMap();
    glViewport(
        0,
        0,
        shadowMap->getShadowWidth(),
        shadowMap->getShadowHeight());

    shadowMap->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModelLocation = directLightShadowMapShader.getUniformModelLocation();
    directLightShadowMapShader.setDirectionalLightTransform(light->computeProjectionViewLightTransform());
    
    // Render the whole scene
    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(const glm::mat4 &projection, const glm::mat4 &view)
{
    // Activate the required shader for drawing
    shaderManagers[0].useShader();

    uniformModelLocation = shaderManagers[0].getUniformModelLocation();
    glViewport(0, 0, 1334, 768);
    
    // Color to be used for clearing the window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear both the color buffer as well as the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setting up the view and projection matrices
    glUniformMatrix4fv(
        shaderManagers[0].getUniformViewLocation(),
        1,
        GL_FALSE,
        glm::value_ptr(view));

    glUniformMatrix4fv(
        shaderManagers[0].getUniformProjectionLocation(),
        1,
        GL_FALSE,
        glm::value_ptr(projection));

    // Set camera position in the shader
    // for specular lighting calculations
    glUniform3f(
        shaderManagers[0].getUniformCameraPosition(),
        camera.getCameraPosition().x,
        camera.getCameraPosition().y,
        camera.getCameraPosition().z);
    
    // Adding lights to the scene
    shaderManagers[0].setDirectionalLightData(&directionalLight);
    shaderManagers[0].setPointLightsData(pointLights, numberOfPointLights);
    shaderManagers[0].setSpotLightsData(spotLights, numberOfSpotLights);
    shaderManagers[0].setDirectionalLightTransform(directionalLight.computeProjectionViewLightTransform());

    directionalLight.getShadowMap()->read(GL_TEXTURE1);
    shaderManagers[0].setPrimaryTexture(0);
    shaderManagers[0].setDirectionalLightShadowMap(1);

    RenderScene();
}

int main()
{
    // Create a glfw window with an OpenGL context
    window = WindowManager();
    if (!window.createWindow(800, 600, "Shadow Mapping"))
    {
        printf("Error: Window was not created, exiting the program!\n");
        return 1;
    }
//--------------------------------------------------------------------------------------------
    // Generate the meshes and shaders
    CreateMeshes();

    // Load models off the disk
    xWing = Model();
    xWing.loadModel("./scenes/shadow-mapping/assets/models/x-wing.obj");

    blackhawk = Model();
    blackhawk.loadModel("./scenes/shadow-mapping/assets/models/uh60.obj");

    CreateShaderPrograms();

    // Generate a camera with default
    // parameters to navigate through the scene
    camera = Camera();

    // Placeholder object for the camera's view matrix
    glm::mat4 view(1.0f);

    // Initial previous time stamp value for delta 
    // time calculations in the render loop
    GLfloat previousTimeStamp = 0.0f;

    // Use GLM's perspective function to create the projection matrix
    // Arguments: Fov of the frustum in degrees, aspect ratio, 
        // distance from camera to near plane, distance from camera to the far plane
    glm::mat4 projection = glm::perspective(
                                45.0f,
                                window.getBufferAspectRatio(),
                                0.1f,
                                100.0f);
//--------------------------------------------------------------------------------------------
    // Load texture
    brickTexture = Texture();
    brickTexture.createTexture("./scenes/shadow-mapping/assets/textures/brick.png");
    if (!brickTexture.loadTextureWithAlpha())
    {
        printf("Error: main(): Failed to load the brick texture!\n");
        return 1;
    }

    dirtTexture = Texture();
    dirtTexture.createTexture("./scenes/shadow-mapping/assets/textures/dirt.png");
    if (!dirtTexture.loadTextureWithAlpha())
    {
        printf("Error: main(): Failed to load the dirt texture!\n");
        return 1;
    }

    plainTexture = Texture();
    plainTexture.createTexture("./scenes/shadow-mapping/assets/textures/plain.png");
    if (!plainTexture.loadTextureWithAlpha())
    {
        printf("Error: main(): Failed to load the plain texture!\n");
        return 1;
    }
//--------------------------------------------------------------------------------------------
    // Initialising the lights in the scene
    // Initialise a direct light
    directionalLight = DirectionalLight();
    directionalLight.computeShadowMap();

    directionalLight.setDirectLightDirection(glm::vec3(0.0f, -15.0f, 10.0f));
    directionalLight.setAmbientLightIntensity(0.1f);
    directionalLight.setDiffuseLightIntensity(0.8f);
//--------------------------------------------------------------------------------------------
    // Initialise the materials for the objects
    shinyMaterial = Material();
    dullMaterial = Material();

    shinyMaterial.setSpecularIntensity(4.0f);
    shinyMaterial.setShininess(256.0f);
    dullMaterial.setSpecularIntensity(0.5f);
    dullMaterial.setShininess(4.0f);
//--------------------------------------------------------------------------------------------
    // Loop until window is closed, a.k.a rendering loop
    while (!window.isWindowClosed())
    {
        // Get and handle user input events
        glfwPollEvents();

        // Generate the delta time for current render loop iteration
        GLfloat currentTimeStamp = glfwGetTime();
        GLfloat deltaTime = currentTimeStamp - previousTimeStamp;
        previousTimeStamp = currentTimeStamp;

        // Update camera parameters based on user inputs and generate
        // the view matrix.
        camera.updateCameraMotion(window.getKeys(), deltaTime);
        camera.updateCameraOrientation(window.getXChange(), window.getYChange());
        camera.generateViewMatrix(view);

        RenderDirectLightShadowMap(&directionalLight);
        RenderPass(projection, view);    

        // Deactivating shaders for completeness
        glUseProgram(0);

        // Swap buffers after drawing to update the viewport
        window.swapBuffers();
    }

    return 0;
}
