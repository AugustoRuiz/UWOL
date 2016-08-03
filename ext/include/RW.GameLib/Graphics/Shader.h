#pragma once

#include "../Utils/Log.h"
#include "../Utils/Pack.h"
#include "GLFuncs.h"
#include <string>
#include <vector>
#include <map>

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
