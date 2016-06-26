#pragma once

#include <vector>
#include <sstream>
#include "GLFuncs.h"
#include "Shader.h"
#include "ShaderMgr.h"

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

using namespace std;

class Program
{
public:
	Program();
	Program(vector<string> vertexShaders, vector<string> fragmentShaders);
	Program(vector<Shader*> shaders);
	~Program();

	GLuint ProgramId;
	
	void AddShader(const string &shaderFile, ShaderType type);
	void AddShader(Shader* shader);
	void RemoveShader(Shader* shader);

	void Compile();
	void Use();

	void BindTextures();
	void BindAttribLocation(GLuint location, const string &name);
	void BindFragDataLocation(GLuint location, const string &name);

	SHADERLIST Shaders;
	vector<TEXTUREINFO*> Textures;

	void SetUniform(const string &name, float x, float y, float z);
	void SetUniform(const string &name, const vec2 & v);
	void SetUniform(const string &name, const vec3 & v);
	void SetUniform(const string &name, const vec4 & v);
	void SetUniform(const string &name, const mat4 & m);
	void SetUniform(const string &name, const mat3 & m);
	void SetUniform(const string &name, float val);
	void SetUniform(const string &name, int val);
	void SetUniform(const string &name, bool val);
	void SetUniform(const string &name, GLuint val);

	void PrintActiveUniforms();
	void PrintActiveUniformBlocks();
	void PrintActiveAttribs();

protected:
	std::map<string, int> _uniformLocations;
	ShaderMgr* _shaderMgr;

	int getUniformLocation(const string& name);
	const string getTypeString(GLenum type);

	bool _inUse;

	static vector<Program*> _programs;
};

