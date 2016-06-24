#include "ShaderMgr.h"

ShaderMgr::ShaderMgr(void)
{
}

ShaderMgr::~ShaderMgr(void)
{
}

ShaderMgr * ShaderMgr::GetInstance()
{
	return &_instance;
}

Shader* ShaderMgr::LoadShader(const string &sFileName, ShaderType shaderType)
{
	Shader* result = NULL;

	Log::Out << "ShaderMgr: Loading shader '" << sFileName << "'..." << endl;

	if (cache.find(sFileName) != cache.end())
	{
		Log::Out << "IN CACHE!" << endl;
		result = cache[sFileName];
	}
	else
	{
		result = new Shader();
		result->LoadFile(sFileName, shaderType);
		cache[sFileName] = result;
		Log::Out << result->ShaderId << endl;
	}

	return result;
}

void ShaderMgr::DeleteShaders()
{
	GLFuncs *gl = GLFuncs::GetInstance();

	SHADERMAP::iterator iter;

	iter = cache.begin();
	while (iter != cache.end())
	{
		Log::Out << "ShaderMgr: Deleting shader #" << iter->second->ShaderId << "." << endl;

		gl->DeleteShader(iter->second->ShaderId);
		iter++;
	}
	cache.clear();
}

GLuint ShaderMgr::CreateProgram(const SHADERLIST& shaderList) {
	vector<GLuint> shaderIds;
	for (Shader* s : shaderList) {
		shaderIds.push_back(s->ShaderId);
	}
	return GLFuncs::GetInstance()->CreateProgram(shaderIds);
}

void ShaderMgr::BindAttribLocation(GLuint programId, GLuint location, const string &name) {
	glBindAttribLocation(programId, location, name.c_str());
}

void ShaderMgr::BindFragDataLocation(GLuint programId, GLuint location, const string &name) {
	glBindFragDataLocation(programId, location, name.c_str());
}
