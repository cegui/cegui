#include "CEGUI/RendererModules/OpenGL3/Shader.h"
#include "cegui/Logger.h"
#include <GL/glew.h>

#include <sstream>

namespace CEGUI
{

    Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source) :
createdSucessfully(false),
    m_program(0),
    m_geometryShader(0),
    m_vertexShader(0),
    m_fragmentShader(0),
    m_shaderName("")
{
    // Compile the shaders

    m_vertexShader = compile(GL_VERTEX_SHADER, vertex_shader_source, m_shaderName + ".vert");
    if (m_vertexShader == 0)
        return;
    
    checkGLErrors();

    if(fragment_shader_source.length() > 0)
    {
        m_fragmentShader = compile(GL_FRAGMENT_SHADER, fragment_shader_source, m_shaderName + ".frag");

        if (m_fragmentShader == 0)
            return;
    }

    checkGLErrors();

    m_program = glCreateProgram(); 
}

Shader::~Shader()
{
    if(m_program != NULL)
        glDeleteProgram(m_program);
    if(m_vertexShader != NULL)
        glDeleteShader(m_vertexShader);
    if(m_fragmentShader != NULL)
        glDeleteShader(m_fragmentShader);
    if(m_geometryShader != NULL)
        glDeleteShader(m_geometryShader);
}


// Bind the shader to the OGL state-machine
void Shader::bind() const
{
    glUseProgram(m_program);
}

// Unbind the shader
void Shader::unbind() const
{
    glUseProgram(0);
}

// Query the location of a vertex attribute inside the shader.
int Shader::get_attrib_location(const std::string &name) const
{
    return glGetAttribLocation(m_program, name.c_str());
}

// Query the location of a uniform variable inside the shader.
int Shader::get_uniform_location(const std::string &name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

// Define the name of the variable inside the shader which represents the final color for each fragment.
void Shader::bind_frag_data_location(const std::string &name)
{
    if(m_program > 0)
    {
        glBindFragDataLocation(m_program, 0, name.c_str() );
        link();
    }
}

bool Shader::isCreatedSuccessfully()
{
    return createdSucessfully;
}


int Shader::compile (int type, const string &source, const string &fileName)
{
    // Create shader object
    checkGLErrors();
    GLuint shader = glCreateShader(type);

    if (shader == 0)
    {
        stringstream stringStream;
        stringStream << "Could not create shader object of type:" << type << ".";
        CEGUI::Logger::getSingleton().logEvent(stringStream.str());
        return 0;
    }

    checkGLErrors();

    // Define shader source and compile

    const char* src = source.data();
    int len = source.size();

    glShaderSource(shader, 1, &src, &len);

    glCompileShader(shader);

    // Check for errors

    int status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        outputShaderLog(shader);
        return 0;
    }

    checkGLErrors();

    return shader;    
}

void Shader::link()
{

    // Attach shaders and link
    glAttachShader(m_program, m_vertexShader);

    if(m_geometryShader != 0)
        glAttachShader(m_program, m_geometryShader);

    if(m_fragmentShader !=0)
        glAttachShader(m_program, m_fragmentShader);

    glLinkProgram(m_program);

    // Check for problems

    int status;

    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        CEGUI::Logger::getSingleton().logEvent("Shader linking has failed.");
        
        outputProgramLog(m_program);

        glDeleteProgram(m_program);
        m_program = 0;
    }

    checkGLErrors();

    if (m_program == 0)
        return;

    createdSucessfully = true;
    checkGLErrors();


    glBindFragDataLocation(m_program, 0, "out0"); // GL_COLOR_ATTACHMENT0
    glBindFragDataLocation(m_program, 1, "out1"); // GL_COLOR_ATTACHMENT1
    glBindFragDataLocation(m_program, 2, "out2"); // ...
    glBindFragDataLocation(m_program, 3, "out3");
    glBindFragDataLocation(m_program, 4, "out4");
    glBindFragDataLocation(m_program, 5, "out5");
    glBindFragDataLocation(m_program, 6, "out6");
    glBindFragDataLocation(m_program, 7, "out7");
    checkGLErrors();
}

#define LOG_BUFFER_SIZE 8096

void Shader::outputProgramLog(int program)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetProgramInfoLog(program, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0) {
        CEGUI::Logger::getSingleton().logEvent(logBuffer);
    }
};

void Shader::outputShaderLog(int shader)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0)
    {
        stringstream ss;
        ss << "Shader compilation has failed.\n" << logBuffer;
        CEGUI::Logger::getSingleton().logEvent(ss.str());
    }
};

void my_get_errors(const char *location)
{
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        stringstream stringStream;
        stringStream << "Error at " << location << ": " << endl; 

        switch (error)
        {
        case GL_INVALID_ENUM:
            stringStream << "GL_INVALID_ENUM: enum argument out of range." << endl;
            break;
        case GL_INVALID_VALUE:
            stringStream << "GL_INVALID_VALUE: Numeric argument out of range." << endl;
            break;
        case GL_INVALID_OPERATION:
            stringStream << "GL_INVALID_OPERATION: Operation illegal in current state." << endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            stringStream << "GL_INVALID_FRAMEBUFFER_OPERATION: Framebuffer object is not complete." << endl;
            break;
        case GL_OUT_OF_MEMORY:
            stringStream << "GL_OUT_OF_MEMORY: Not enough memory left to execute command." << endl;
            break;
        default:
            stringStream << "GL_ERROR: Unknown error." << endl;
        }

         CEGUI_THROW(RendererException(stringStream.str().c_str()));
    }
}


}