#include "shader-manager.h"

ShaderManager::ShaderManager(): 
    m_shaderProgramID(0),
    m_uniformModelLocation(0),
    m_uniformProjectionLocation(0)
{
}

void ShaderManager::createShaderProgramFromStrings(
        const char* vertexShaderCode,
        const char* fragmentShaderCode)
{
    compileShader(vertexShaderCode, fragmentShaderCode);
}

void ShaderManager::createShaderProgramFromFiles(
    const char* vertexShaderPath,
    const char* fragmentShaderPath)
{
    std::string vertexShaderString, fragmentShaderString;
    readShaderFile(vertexShaderPath, vertexShaderString);
    readShaderFile(fragmentShaderPath, fragmentShaderString);
    compileShader(vertexShaderString.c_str(), fragmentShaderString.c_str());
}

void ShaderManager::readShaderFile(const char* filePath, std::string &contents)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        printf(
            "Error: Failed to read file at %s , file does not exist.",
            filePath);
        return;
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        contents.append(line + "\n");
    }

    fileStream.close();
}

void ShaderManager::compileShader(
        const char* vertexShaderCode,
        const char* fragmentShaderCode)
{
    // Create an empty shader program object
    m_shaderProgramID = glCreateProgram();

    // Check if it was created successfully
    if(!m_shaderProgramID)
    {
        printf("Error: Generation of shader program failed!\n");
        return;
    }

    // Attaching our vertex and fragment shaders to the shader program
    AddShader(vertexShaderCode, GL_VERTEX_SHADER);
    AddShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    // Setting up error logging objects
    GLint result = 0;
    GLchar log[1024] = { 0 };

    // Perform shader program linking
    glLinkProgram(m_shaderProgramID);

    // Find and log errors if any from the linking process done above
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(m_shaderProgramID, sizeof(log), NULL, log);
        printf("Error: Linking of the shader program failed, '%s'\n", log);
        return;
    }

    // Perform shader program validation
    glValidateProgram(m_shaderProgramID);

    // Find and log errors if any from the validation process done above
    glGetProgramiv(m_shaderProgramID, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(m_shaderProgramID, sizeof(log), NULL, log);
        printf("Error: Shader program validation failed, '%s'", log);
        return;
    }

    // Get the location of the uniform variable to
    // provide the transform information to the shader
    m_uniformModelLocation = glGetUniformLocation(m_shaderProgramID, "model");
    m_uniformProjectionLocation = glGetUniformLocation(m_shaderProgramID, "projection");
}

void ShaderManager::AddShader(
    const char* shaderSource,
    GLenum shaderType)
{
    // Create an empty shader object
    GLuint shaderID = glCreateShader(shaderType);

    // Convert the source code for the shader 
    // into the right type for loading
    const GLchar* sourceCode[1];
    sourceCode[0] = shaderSource;

    // Convert the length of the source code for 
    // the shader into the right type for loading
    GLint sourceLength[1];
    sourceLength[0] = strlen(shaderSource);

    // Load the shader source into the 
    // empty shader object and compile the code
    glShaderSource(shaderID, 1, sourceCode, sourceLength);
    glCompileShader(shaderID);

    // Find and log error if any from 
    // the above compilation process
    GLint result = 0;
    GLchar log[1024] = { 0 };

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
        printf("Error: Compilation of shader of type %d failed, '%s'\n", shaderType, log);
        return;
    }

    // Attach the shader to the shader program
    glAttachShader(m_shaderProgramID, shaderID);
}

void ShaderManager::useShader()
{
    glUseProgram(m_shaderProgramID);
}

void ShaderManager::clearShader()
{
    if(m_shaderProgramID)
    {
        // Free the shader program from memory (graphics)
        glDeleteProgram(m_shaderProgramID);
        m_shaderProgramID = 0;
    }

    m_uniformModelLocation = 0;
    m_uniformProjectionLocation = 0;
}

ShaderManager::~ShaderManager()
{
    // Ensuring the shader is off the graphics card's 
    // memory before deleting the shader manager object
    clearShader();
}
