#pragma once

#include "GLFuncs.h"
#include <map>
#include <string>

enum ShaderType {
	Vertex,
	Fragment
};

class Shader {
public:
	GLuint ShaderId;
	void LoadFile(const std::string& shaderFile, ShaderType shaderType);
	void Load(const std::string& shaderText, ShaderType shaderType);
};

typedef vector<Shader*> SHADERLIST;
typedef map<string, Shader*> SHADERMAP;
