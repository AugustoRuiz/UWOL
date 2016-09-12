#include "Graphics/Program.h"

Program::Program() {
	this->_g = GLFuncs::GetInstance();
	this->_shaderMgr = ShaderMgr::GetInstance();
	Program::_programs.push_back(this);
	this->_inUse = false;
}

Program::Program(vector<string> vertexShaders, vector<string> fragmentShaders) : Program() {
	for (string shaderFile : vertexShaders) {
		this->AddShader(shaderFile, ShaderType::Vertex);
	}
	for (string shaderFile : fragmentShaders) {
		this->AddShader(shaderFile, ShaderType::Fragment);
	}
	this->Compile();
}

Program::Program(vector<Shader*> shaders) : Program() {
	for (Shader* shader : shaders) {
		this->AddShader(shader);
	}
	this->Compile();
}

void Program::Use() {
	if (!this->_inUse) {
		if(this->ProgramId != 0) {
			_g->UseProgram(this->ProgramId);
			for (Program* p : Program::_programs) {
				p->_inUse = (this == p);
			}
			this->_inUse = true;
		}
	}
}

vector<Program*> Program::_programs;

Program::~Program()
{
}

void Program::AddShader(const string &shaderFile, ShaderType type) {
	this->AddShader(ShaderMgr::GetInstance()->LoadShader(shaderFile, type));
}

void Program::AddShader(Shader* shader) {
	this->Shaders.push_back(shader);
}

void Program::RemoveShader(Shader* shader) {
	SHADERLIST::iterator pos = this->Shaders.end();
	for (SHADERLIST::iterator it = this->Shaders.begin(); it < pos; ++it) {
		if ((*it)->ShaderId == shader->ShaderId) {
			pos = it;
		}
	}
	if (pos != this->Shaders.end()) {
		this->Shaders.erase(pos);
	}
}

void Program::Compile() {
	this->ProgramId = ShaderMgr::GetInstance()->CreateProgram(this->Shaders);
}

void Program::BindAttribLocation(GLuint location, const string &name)
{
	ShaderMgr::GetInstance()->BindAttribLocation(this->ProgramId, location, name);
}

void Program::BindFragDataLocation(GLuint location, const string &name)
{
	ShaderMgr::GetInstance()->BindFragDataLocation(this->ProgramId, location, name);
}

void Program::BindTextures() {
	for (int i = 0, li = this->Textures.size(); i < li; ++i) {
		_g->SetTexture(i, this->Textures[i]->texture);

		stringstream ss;
		ss << "iChannel" << i;
		this->SetUniform(ss.str(), i);
	}
}

void Program::SetUniform(const string &name, float x, float y, float z)
{
	_g->SetUniform(this->ProgramId, name, x, y, z);
}

void Program::SetUniform(const string &name, const vec3 & v)
{
	_g->SetUniform(this->ProgramId, name, v);
}

void Program::SetUniform(const string &name, const vec4 & v)
{
	_g->SetUniform(this->ProgramId, name, v);
}

void Program::SetUniform(const string &name, const vec2 & v)
{
	_g->SetUniform(this->ProgramId, name, v);
}

void Program::SetUniform(const string &name, const mat4 & m)
{
	_g->SetUniform(this->ProgramId, name, m);
}

void Program::SetUniform(const string &name, const mat3 & m)
{
	_g->SetUniform(this->ProgramId, name, m);
}

void Program::SetUniform(const string &name, float val)
{
	_g->SetUniform(this->ProgramId, name, val);
}

void Program::SetUniform(const string &name, int val)
{
	_g->SetUniform(this->ProgramId, name, val);
}

void Program::SetUniform(const string &name, GLuint val)
{
	_g->SetUniform(this->ProgramId, name, val);
}

void Program::SetUniform(const string &name, bool val)
{
	_g->SetUniform(this->ProgramId, name, val);
}

void Program::PrintActiveUniforms() {
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveUniform
	GLint nUniforms, size, location, maxLen;
	GLchar * name;
	GLsizei written;
	GLenum type;

	glGetProgramiv(this->ProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(this->ProgramId, GL_ACTIVE_UNIFORMS, &nUniforms);

	name = new GLchar[maxLen];

	Log::Out << "Active uniforms:" << endl;
	Log::Out << "------------------------------------------------" << endl;
	for (int i = 0; i < nUniforms; ++i) {
		glGetActiveUniform(this->ProgramId, i, maxLen, &written, &size, &type, name);
		location = glGetUniformLocation(this->ProgramId, name);
		Log::Out << setfill('0') << setw(5) << location << " " << name << "(" << getTypeString(type) << ")" << endl;
	}

	delete[] name;
#else
	// For OpenGL 4.3 and above, use glGetProgramResource
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(this->ProgramId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	Log::Out << "Active uniforms:" << endl;
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		glGetProgramResourceiv(this->ProgramId, GL_UNIFORM, i, 4, properties, 4, NULL, results);

		if (results[3] != -1) continue;  // Skip uniforms in blocks
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(this->ProgramId, GL_UNIFORM, i, nameBufSize, NULL, name);
		Log::Out << setfill('0') << setw(5) << results[2] << " " << name << "(" << getTypeString(results[1]) << ")" << endl;
		delete[] name;
	}
#endif
}

void Program::PrintActiveUniformBlocks() {
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveUniformBlockiv
	GLint written, maxLength, maxUniLen, nBlocks, binding;
	GLchar * name;

	glGetProgramiv(this->ProgramId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxLength);
	glGetProgramiv(this->ProgramId, GL_ACTIVE_UNIFORM_BLOCKS, &nBlocks);
	glGetProgramiv(this->ProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniLen);
	GLchar * uniName = new GLchar[maxUniLen];
	name = new GLchar[maxLength];

	Log::Out << "Active Uniform blocks:" << endl;
	Log::Out << "------------------------------------------------" << endl;
	for (int i = 0; i < nBlocks; i++) {
		glGetActiveUniformBlockName(this->ProgramId, i, maxLength, &written, name);
		glGetActiveUniformBlockiv(this->ProgramId, i, GL_UNIFORM_BLOCK_BINDING, &binding);
		Log::Out << "Uniform block \"" << name << "\" (" << binding << "):" << endl;

		GLint nUnis;
		glGetActiveUniformBlockiv(this->ProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nUnis);
		GLint * unifIndexes = new GLint[nUnis];
		glGetActiveUniformBlockiv(this->ProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, unifIndexes);

		for (int unif = 0; unif < nUnis; ++unif) {
			GLint uniIndex = unifIndexes[unif];
			GLint size;
			GLenum type;

			glGetActiveUniform(this->ProgramId, uniIndex, maxUniLen, &written, &size, &type, uniName);
			Log::Out << "    " << name << " (" << getTypeString(type) << ")" << endl;
		}

		delete[] unifIndexes;
	}
	delete[] name;
	delete[] uniName;
#else
	GLint numBlocks = 0;

	glGetProgramInterfaceiv(this->ProgramId, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
	GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
	GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
	GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

	for (int block = 0; block < numBlocks; ++block) {
		GLint blockInfo[2];
		glGetProgramResourceiv(this->ProgramId, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
		GLint numUnis = blockInfo[0];

		char * blockName = new char[blockInfo[1] + 1];
		glGetProgramResourceName(this->ProgramId, GL_UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);
		Log::Out << "Uniform block \"" << blockName << "\":" << endl;
		delete[] blockName;

		GLint * unifIndexes = new GLint[numUnis];
		glGetProgramResourceiv(this->ProgramId, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

		for (int unif = 0; unif < numUnis; ++unif) {
			GLint uniIndex = unifIndexes[unif];
			GLint results[3];
			glGetProgramResourceiv(this->ProgramId, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			glGetProgramResourceName(this->ProgramId, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);
			Log::Out << "    " << name << " (" << getTypeString(results[1]) << ")" << endl;
			delete[] name;
		}

		delete[] unifIndexes;
	}
#endif
}

void Program::PrintActiveAttribs() {
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveAttrib
	GLint written, size, location, maxLength, nAttribs;
	GLenum type;
	GLchar * name;

	glGetProgramiv(this->ProgramId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	glGetProgramiv(this->ProgramId, GL_ACTIVE_ATTRIBUTES, &nAttribs);

	name = new GLchar[maxLength];
	Log::Out << "Active Attributes:" << endl;
	Log::Out << "------------------------------------------------" << endl;
	for (int i = 0; i < nAttribs; i++) {
		glGetActiveAttrib(this->ProgramId, i, maxLength, &written, &size, &type, name);
		location = glGetAttribLocation(this->ProgramId, name);
		Log::Out << setfill('0') << setw(5) << location << " " << name << "(" << getTypeString(type) << ")" << endl;
	}
	delete[] name;
#else
	// >= OpenGL 4.3, use glGetProgramResource
	GLint numAttribs;
	glGetProgramInterfaceiv(this->ProgramId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	Log::Out << "Active attributes:" << endl;
	for (int i = 0; i < numAttribs; ++i) {
		GLint results[3];
		glGetProgramResourceiv(this->ProgramId, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(this->ProgramId, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
		Log::Out << setfill('0') << setw(5) << results[2] << " " << name << " (" << getTypeString(results[1]) << ")" << endl;
		delete[] name;
	}
#endif
}

const string Program::getTypeString(GLenum type) {
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch (type) {
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}