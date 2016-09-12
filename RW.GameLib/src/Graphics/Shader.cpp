#include "Graphics/Shader.h"

void Shader::LoadFile(const std::string & shaderFile, ShaderType shaderType)
{
	std::string fileStr = Pack::GetInstance()->GetString(shaderFile);
	this->Load(fileStr, shaderType);
}

void Shader::Load(const std::string & shaderText, ShaderType shaderType)
{
	GLenum glShaderType = GL_FRAGMENT_SHADER;
	switch (shaderType) {
		case Vertex:
			glShaderType = GL_VERTEX_SHADER;
			break;
		case Fragment:
			glShaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			glShaderType = GL_FRAGMENT_SHADER;
			break;
	}
	this->ShaderId = GLFuncs::GetInstance()->CreateShader(glShaderType, shaderText);
}
