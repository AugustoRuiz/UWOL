#include "Graphics/GLFuncs.h"

GLFuncs *GLFuncs::GetInstance()
{
	return &_instance;
}

void GLFuncs::SetPack(Pack* pack) {
	this->_pack = pack;
}

SDL_Window *GLFuncs::Initialize(int screenWidth, int screenHeight, GLboolean fullscreen, const char* name)
{
	Uint32 flags;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Log::Out << "Unable to initialize SDL: " << SDL_GetError() << endl;
		return NULL;
	}
	atexit(SDL_Quit);

	flags = SDL_WINDOW_OPENGL;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_windowWidth = _screenWidth;
	_windowHeight = _screenHeight;

	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;

		SDL_DisplayMode currentDisplay;
		if(SDL_GetCurrentDisplayMode(0, &currentDisplay)) {
			Log::Out << "Couldn't get current display mode: " << SDL_GetError() << endl;
			_realWidth = _screenWidth;
			_realHeight = _screenHeight;
		} else {
			Log::Out << "Current display mode: " << currentDisplay.w << "x" << currentDisplay.h << endl;
			double desiredRatio = (double)screenWidth / screenHeight;
			double realRatio = (double)currentDisplay.w / currentDisplay.h;
			if(desiredRatio == realRatio) {
				_realWidth = currentDisplay.w;
				_realHeight = currentDisplay.h;
			} else if(desiredRatio > realRatio) {
				// real screen is taller
				_realWidth = currentDisplay.w;
				_realHeight = currentDisplay.h * ((double)screenWidth/currentDisplay.w);
			} else {
				// real screen is wider
				_realHeight = currentDisplay.h;
				_realWidth = currentDisplay.w * ((double)screenHeight/currentDisplay.h);
			}
			_windowWidth = currentDisplay.w;
			_windowHeight = currentDisplay.h;
		}
	} else {
		_realWidth = _screenWidth;
		_realHeight = _screenHeight;
	}

	Log::Out << "Real size: " << _realWidth << "x" << _realHeight << endl;
	Log::Out << "Window size: " << _windowWidth << "x" << _windowHeight << endl;

	Log::Out << "Offset: " << (_windowWidth - _realWidth) / 2 << "x" << (_windowHeight - _realHeight) / 2 << endl;

	setGLAttributes();

	_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_windowWidth, _windowHeight,
		flags);

	if (!_window) {
		Log::Out << "Unable to create window." << endl;
		checkSDLError(__LINE__);
	}

	SDL_Surface *icon = Pack::GetInstance()->GetImg("data/UWOLIcon.png");
	SDL_SetWindowIcon(_window, icon);

	_mainContext = SDL_GL_CreateContext(_window);
	SDL_GL_MakeCurrent(_window, _mainContext);

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	char* obtainedVersion = (char*)glGetString(GL_VERSION);
	string glVersion = getVersionFromStr(obtainedVersion);

	Log::Out << "OpenGL: " << obtainedVersion << endl;

	int version;
	stringstream ss(glVersion);
	ss >> version;

#ifdef __APPLE__
	this->_useShaders = true;
#else
	this->_useShaders = (glCreateProgram != NULL); //version >= 20;
#endif
	if (_useShaders) {
		this->_vtxAttribIdx = 0;
		this->_uvAttribIdx = 1;
		this->_colAttribIdx = 2;

		this->_glslVersion = getGLSLVersion();
		Log::Out << "GLSL: " << (this->_glslVersion == "" ? "Not available!" : this->_glslVersion) << endl;
	}
	else {
		Log::Out << "Shaders are not available" << endl;
	}

	this->_useVBO = (glGenVertexArrays!=NULL); //version >= 20;
	if (this->_useVBO) {
		this->_useFramebuffer = this->initFramebuffer();
	}

	glViewport(0, 0, _screenWidth, _screenHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	//	glShadeModel(GL_SMOOTH);

	glDisable(GL_CULL_FACE);

	//#if !ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
	//	//glEnable(GL_ALPHA_TEST);
	//	//glAlphaFunc(GL_GREATER, 0.01f);
	//	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(2.0f);

	if (this->_useVBO) {
		glGenVertexArrays(1, &_vaoVertex);
		glBindVertexArray(_vaoVertex);

		glGenBuffers(1, &_vboVertex);
		glGenBuffers(1, &_vboUV);
		glGenBuffers(1, &_vboColor);
		glGenBuffers(1, &_vboLineVertex);
	}

	this->ResetMVP();
	this->StaticProjection = glm::ortho(0.0f, (float)_windowWidth, (float)_windowHeight, 0.0f, -1.0f, 1.0f);

	GLint texture_units;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	_activeTextures.resize(texture_units, 0);

	return _window;
}

void GLFuncs::GetRealSize(int *realWidth, int *realHeight) {
	*realWidth = _windowWidth;
	*realHeight = _windowHeight;
}

bool GLFuncs::CanUseShaders() {
	return _useShaders;
}

SDL_Window* GLFuncs::GetWindow() {
	return _window;
}

bool GLFuncs::initFramebuffer() {
	if (((unsigned long)glGenFramebuffers) == 0) {
		Log::Out << "glGenFramebuffers is not available." << endl;
		return false;
	}
	Log::Out << "Creating framebuffer" << endl;
	// Inicializemos el framebuffer para poder renderizar sobre una textura, así podremos aplicar shaders 
	// a lo que pintemos. ;)

	glGenFramebuffers(1, &_frameBufferName);
	Log::Out << "Framebuffer #" << _frameBufferName << endl;
	if (_frameBufferName == 0) {
		Log::Out << "Call to glGenFramebuffers failed. " << getErrorString(glGetError()) << endl;
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);
	Log::Out << "Framebuffer bound!" << endl;

	glGenTextures(1, &_renderedTexture);
	glBindTexture(GL_TEXTURE_2D, _renderedTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	Log::Out << "Texture created" << endl;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderedTexture, 0);
	Log::Out << "Texture #" << _renderedTexture << " attached to framebuffer!" << endl;

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Out << "Unable to initialize Framebuffer! " << getErrorString(glGetError()) << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);
	Log::Out << "Framebuffer initialized Ok! " << _frameBufferName << endl;
	return true;
}

string GLFuncs::getErrorString(GLuint error) {
	switch (error) {
	case GL_NO_ERROR:
		return "Ok";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM. An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
#if !ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
#endif
	default:
		stringstream ss;
		ss << "Unknown code: " << error;
		return ss.str();
	}
}

void GLFuncs::setGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);		//Use at least 8 bits of Red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);	    //Use at least 8 bits of Green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);		//Use at least 8 bits of Blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		//Use at least 8 bits of Alpha
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	    //Use at least 16 bits for the depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//Enable double buffering

#ifdef __APPLE__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
}

void GLFuncs::checkSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		Log::Out << "SDL Error: " << error << endl;
		if (line != -1) {
			Log::Out << "Line: " << line << endl;
		}
		SDL_ClearError();
	}
}

void GLFuncs::ResetMVP() {
	this->MVP = glm::ortho(0.0f, (float)_screenWidth, (float)_screenHeight, 0.0f) * glm::mat4(1.0f) * glm::mat4(1.0f);
}

void GLFuncs::OffsetMVP(float offsetX, float offsetY) {
	this->MVP = glm::translate(this->MVP, glm::vec3(offsetX, offsetY, 0.0f));
}

void GLFuncs::SetTexture(unsigned int channel, unsigned int texture) {
	if(!this->_useShaders) {
		glEnable(GL_TEXTURE_2D);
	}
	
	if (_activeTextures[channel] != texture) {
		if (this->_useShaders) {
			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, texture);
			glActiveTexture(GL_TEXTURE0);
		}
		else {
#ifndef __APPLE__
			if (glActiveTexture != NULL) {
#endif
				glActiveTexture(GL_TEXTURE0 + channel);
#ifndef __APPLE__
			}
#endif
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		_activeTextures[channel] = texture;
	}
}

void GLFuncs::BlitColoredRect(int iX, int iY, int width, int height,
	float tx1, float ty1, float tx2, float ty2, float red,
	float green, float blue, float alpha, bool additive)
{
	if (additive) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	GLfloat vertex_buffer_data[] = {
		(float)iX, (float)iY, 0.0f,
		(float)iX + width, (float)iY, 0.0f,
		(float)iX + width, (float)iY + height, 0.0f,
		(float)iX, (float)iY + height, 0.0f
	};

	GLfloat uv_buffer_data[] = {
		tx1, ty1,
		tx2, ty1,
		tx2, ty2,
		tx1, ty2
	};

	GLfloat color_buffer_data[] = {
		red, green, blue, alpha,
		red, green, blue, alpha,
		red, green, blue, alpha,
		red, green, blue, alpha
	};

	this->BlitVerts(vertex_buffer_data, sizeof(vertex_buffer_data),
		uv_buffer_data, sizeof(uv_buffer_data),
		color_buffer_data, sizeof(color_buffer_data));
}

void GLFuncs::BlitVerts(float vertex_buffer_data[], unsigned int vBufSize,
	float uv_buffer_data[], unsigned int uvBufSize,
	float color_buffer_data[], unsigned int cBufSize)
{
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (this->_useVBO) {
		glEnableVertexAttribArray(_vtxAttribIdx);
		glBindBuffer(GL_ARRAY_BUFFER, _vboVertex);
		glBufferData(GL_ARRAY_BUFFER, vBufSize, vertex_buffer_data, GL_STATIC_DRAW);
		glVertexAttribPointer(_vtxAttribIdx, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

		glEnableVertexAttribArray(_uvAttribIdx);
		glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
		glBufferData(GL_ARRAY_BUFFER, uvBufSize, uv_buffer_data, GL_STATIC_DRAW);
		glVertexAttribPointer(_uvAttribIdx, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

		glEnableVertexAttribArray(_colAttribIdx);
		glBindBuffer(GL_ARRAY_BUFFER, _vboColor);
		glBufferData(GL_ARRAY_BUFFER, cBufSize, color_buffer_data, GL_STATIC_DRAW);
		glVertexAttribPointer(_colAttribIdx, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(_colAttribIdx);
		glDisableVertexAttribArray(_uvAttribIdx);
		glDisableVertexAttribArray(_vtxAttribIdx);
	}
	else {
#if !ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&(this->MVP[0][0]));

		glBegin(GL_QUADS);
		glColor4fv(&color_buffer_data[0]);
		glTexCoord2fv(&uv_buffer_data[0]);
		glVertex3fv(&vertex_buffer_data[0]);

		glColor4fv(&color_buffer_data[4]);
		glTexCoord2fv(&uv_buffer_data[2]);
		glVertex3fv(&vertex_buffer_data[3]);

		glColor4fv(&color_buffer_data[8]);
		glTexCoord2fv(&uv_buffer_data[4]);
		glVertex3fv(&vertex_buffer_data[6]);

		glColor4fv(&color_buffer_data[12]);
		glTexCoord2fv(&uv_buffer_data[6]);
		glVertex3fv(&vertex_buffer_data[9]);
		glEnd();
#endif
	}
}

void GLFuncs::BlitRect(int iX, int iY, int width, int height,
	float tx1, float ty1, float tx2, float ty2)
{
	BlitColoredRect(iX, iY, width, height, tx1, ty1, tx2, ty2, 1.0f, 1.0f, 1.0f, 1.0f, false);
}

void GLFuncs::BlitShadow(int iX, int iY, int width, int height,
	float tx1, float ty1, float tx2, float ty2)
{
	BlitColoredRect(iX, iY, width, height, tx1, ty1, tx2, ty2, 0.0f, 0.0f, 0.0f, 0.5f, false);
}

void GLFuncs::BlitRect(int iX, int iY, int width, int height,
	float tx1, float ty1, float tx2, float ty2, float alpha)
{
	BlitColoredRect(iX, iY, width, height, tx1, ty1, tx2, ty2, 1.0f, 1.0f, 1.0f, alpha, false);
}

void GLFuncs::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLFuncs::DrawPolyLine(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha) {
	if (this->_useVBO) {
		vector<GLfloat> coords;
		GLsizei vertexCount = vertexes.size();
		for (VECTOR2 v : vertexes) {
			coords.push_back((float)v.x);
			coords.push_back((float)v.y);
		}

		glEnableVertexAttribArray(_vtxAttribIdx);
		glBindBuffer(GL_ARRAY_BUFFER, _vboLineVertex);
		glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat), &(coords[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(_vtxAttribIdx, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, vertexCount);

		glDisableVertexAttribArray(_vtxAttribIdx);
	}
	else {
#if !ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		for (VECTOR2 v : vertexes) {
			glVertex2i(v.x, v.y);
		}
		glEnd();
#endif
	}
}

GLuint GLFuncs::CreateProgram(const std::vector<GLuint> &shaderList)
{
	if (!_useShaders) {
		return 0;
	}

	GLuint program = glCreateProgram();

	bool anyValidShader = false;

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		if (shaderList[iLoop] != 0) {
			anyValidShader = true;
		}
		glAttachShader(program, shaderList[iLoop]);
	}

	if (!anyValidShader) {
		Log::Out << "No valid shader was specified." << endl;
		return (GLuint)0;
	}

	glBindAttribLocation(program, this->_vtxAttribIdx, "vertexPosition");
	glBindAttribLocation(program, this->_uvAttribIdx, "vertexUV");
	glBindAttribLocation(program, this->_colAttribIdx, "vertexColor");

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		Log::Out << "Linker failure: " << strInfoLog << endl;
		delete[] strInfoLog;
		glDeleteProgram(program);
		program = 0;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

string GLFuncs::getGLSLVersion() {
	// GL_SHADING_LANGUAGE_VERSION returns the version standard version form 
	//  with decimals, but the GLSL version preprocessor directive simply
	//  uses integers (thus 1.10 should 110 and 1.40 should be 140, etc.)
	char* glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	return getVersionFromStr(glslVersion);
}

string GLFuncs::getVersionFromStr(char* str) {
	string result;
	if (str == NULL) {
		result = "";
	}
	else {
		string string_str(str);
		regex re("^(\\d+)\\.(\\d+).*$");

		// This will return a string with an integer with the Major and minor part of the version.
		result = regex_replace(string_str, re, "$1$2");
	}
	return result;
}

GLuint GLFuncs::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	if (!_useShaders) {
		return 0;
	}

	GLuint shader = glCreateShader(eShaderType);

	if (this->_glslVersion == "") {
		this->_glslVersion = getGLSLVersion();
		if (this->_glslVersion == "") {
			this->_glslVersion = "120";
		}
	}

	// Prepend our vertex shader source string with the supported GLSL version so
	//  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
	stringstream ss;
	ss << "#version " << this->_glslVersion << endl << strShaderFile.c_str();

	string shaderTextStr = ss.str();
	const GLchar* shaderText = (GLchar*)shaderTextStr.c_str();
	Log::Out << shaderText << endl;

	glShaderSource(shader, 1, &shaderText, NULL);
	glCompileShader(shader);

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		Log::Out << "Shader compile log:" << log << endl;
		free(log);
	}

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
			//case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		}

		Log::Out << "Compile failure in " << strShaderType << " shader:" << endl << strInfoLog << endl;
		delete[] strInfoLog;
	}

	return shader;
}

void GLFuncs::DeleteShader(GLuint shaderId) {
	if (shaderId != 0) {
		glDeleteShader(shaderId);
	}
}

void GLFuncs::UseProgram(GLuint programId) {
	if (this->_useShaders && this->_currentProgram != programId && programId != 0) {
		glUseProgram(programId);
		this->_currentProgram = programId;
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, float x, float y, float z) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform3f(location, x, y, z);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, const vec2 & v) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform2f(location, v.x, v.y);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, const vec3 & v) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform3f(location, v.x, v.y, v.z);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, const vec4 & v) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform4f(location, v.x, v.y, v.z, v.w);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, const mat4 & m) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniformMatrix4fv(location, 1, GL_FALSE, &(m[0][0]));
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, const mat3 & m) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniformMatrix3fv(location, 1, GL_FALSE, &(m[0][0]));
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, float val) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform1f(location, val);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, int val) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform1i(location, val);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, bool val) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform1i(location, val);
		}
	}
}

void GLFuncs::SetUniform(GLuint programId, const string &name, GLuint val) {
	if (programId != 0) {
		GLint location = getUniformLocation(programId, name);
		if (location != -1) {
			this->UseProgram(programId);
			glUniform1ui(location, val);
		}
	}
}

GLint GLFuncs::getUniformLocation(GLuint programId, const std::string &uniformName) {
	if (programId != 0) {
		map<string, int> programCache = this->_uniformLocationCache[programId];
		map<string, int>::iterator pos;
		pos = programCache.find(uniformName);

		if (pos == programCache.end()) {
			programCache[uniformName] = glGetUniformLocation(programId, uniformName.c_str());
		}

		return programCache[uniformName];
	}
	return -1;
}

GLuint GLFuncs::GetFramebufferTexture() {
	if (this->_useFramebuffer) {
		return this->_renderedTexture;
	}
	else {
		return 0;
	}
}

void GLFuncs::SwapBuffers()
{
	if (this->_useFramebuffer) {
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		// Render on the whole framebuffer, complete from the lower left corner to the upper right
		glViewport(0, 0, _windowWidth, _windowHeight);
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, _renderedTexture);

		// Renderizar la textura.
		//BlitRect(0, 0, _realWidth, _realHeight, 0.0f, 1.0f, 1.0f, 0.0f);
		
		BlitRect((_windowWidth - _realWidth)/2, 
			(_windowHeight - _realHeight)/2, 
			_realWidth, 
			_realHeight, 0.0f, 1.0f, 1.0f, 0.0f);
	}

	SDL_GL_SwapWindow(this->_window);

	if (this->_useFramebuffer) {
		// Restore so we draw in the texture again.
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		glViewport(0, 0, _screenWidth, _screenHeight);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}
