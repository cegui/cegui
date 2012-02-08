#ifndef SHADER_HPP
#define SHADER_HPP

#include "CEGUI/Exceptions.h"

#include <string>

using namespace std;

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class Shader
    {
    public:

        // Loads shaders from files and compiles them.
        // When path is "hello", the files "hello.frag" & "hello.vert"
        // will be loaded.
        Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
        ~Shader();

        // Bind the shader to the OGL state-machine
        void bind() const;

        // Unbind the shader
        void unbind() const;

        // Query the location of a vertex attribute inside the shader.
        int get_attrib_location(const std::string &name) const;

        // Query the location of a uniform variable inside the shader.
        int get_uniform_location(const std::string &name) const;

        // Define the name of the variable inside the shader which represents the final color for each fragment.
        void bind_frag_data_location(const std::string &name);

        bool isCreatedSuccessfully();


        void link();

    private:
        std::string m_shaderName;
        bool createdSucessfully;

        int m_vertexShader;
        int m_fragmentShader;
        int m_geometryShader;
        int m_program;

        int compile(int type, const string &source, const string &path);

        void outputShaderLog(int shader);
        void outputProgramLog(int program);
    };




    
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define checkGLErrors() my_get_errors(AT)



    using namespace std;

    // Query OpenGL errors and print error messages to STDERR.
    void my_get_errors(const char *location);
}

#endif
