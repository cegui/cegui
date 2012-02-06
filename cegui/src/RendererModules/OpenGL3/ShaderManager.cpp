#include "CEGUI/RendererModules/OpenGL3/ShaderManager.h"

#include "CEGUI/RendererModules/OpenGL3/Shader.h"

#include "CEGUI/RendererModules/OpenGL3/StandardShaderVert.h"
#include "CEGUI/RendererModules/OpenGL3/StandardShaderFrag.h"

#include <iostream>

namespace CEGUI
{

	ShaderManager *ShaderManager::m_instance = 0;

	ShaderManager *ShaderManager::getInstance()
	{
		if(m_instance != 0)
			return m_instance;
		else
			return m_instance = new ShaderManager();
	}

	void ShaderManager::destroy()
	{
		if(m_instance)
			delete m_instance;
	}

	ShaderManager::ShaderManager()
	{
		m_shadersInitialised = false;
	}

	ShaderManager::~ShaderManager()
	{
		deinitialiseShaders();
		m_shadersInitialised = false;
	}

	Shader* ShaderManager::getShader(unsigned int id)
	{
		if(m_shaders.find(id) != m_shaders.end())
			return m_shaders[id];
		else
			return 0;
	}

	void ShaderManager::loadShader(unsigned int id, std::string vertexShader, std::string fragmentShader)
	{
		if(m_shaders.find(id) == m_shaders.end())
		{	
			m_shaders[id] = new Shader(vertexShader, fragmentShader);
			m_shaders[id]->link();
		}
	}

	bool ShaderManager::initialiseShaders()
	{
		if(!m_shadersInitialised)
		{
			loadShader(SHADER_ID_STANDARDSHADER, StandardShaderVert, StandardShaderFrag);


			if(!getShader(SHADER_ID_STANDARDSHADER)->isCreatedSuccessfully())
			{	
				//For CEGUI Log << "\n--- One or multiple shader programs weren't created successfully. Exiting program now. ---" << endl;
				return false;
			}

			std::cout << "Finished loading shaders.\n" << std::endl;
			return true;
		}
		return true;
	}

	void ShaderManager::deinitialiseShaders()
	{
		for(shaderContainerType::iterator iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
		{
			delete iter->second;
		}
		m_shaders.clear();
	}

}