#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <map>
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
	class Shader;

	enum ShaderID
	{
		SHADER_ID_STANDARDSHADER,

		SHADER_ID_COUNT
	};

	class ShaderManager {
	public:
		virtual ~ShaderManager();

		static ShaderManager *getInstance();
		static void destroy();

		Shader* getShader(unsigned int id);
		void loadShader(unsigned int id, std::string vertexShader, std::string fragmentShader);

		bool initialiseShaders();
		void deinitialiseShaders();

	private:
		ShaderManager();

		static ShaderManager *m_instance;

		typedef std::map<unsigned int, Shader*> shaderContainerType;
		shaderContainerType m_shaders;

		bool m_shadersInitialised;
	};

}

#endif