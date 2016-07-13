#pragma once

#include "../Utils/Log.h"
#include "GLFuncs.h"
#include "Shader.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class ShaderMgr
{
public:
	ShaderMgr(void);
	~ShaderMgr(void);

	Shader* LoadShader(const string &sFileName, ShaderType type);
	void DeleteShaders();

	static ShaderMgr* GetInstance();

	GLuint CreateProgram(const SHADERLIST& shaderList);

	void BindAttribLocation(GLuint programId, GLuint location, const string &name);
	void BindFragDataLocation(GLuint programId, GLuint location, const string &name);

protected:
	static ShaderMgr _instance;

private:
	SHADERMAP cache;
};
