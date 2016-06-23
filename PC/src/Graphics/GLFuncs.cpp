#include "GLFuncs.h"

GLFuncs *GLFuncs::GetInstance()
{
	return &_instance;
}

SDL_Window *GLFuncs::Initialize(int screenWidth, int screenHeight, GLboolean fullscreen, const char* name)
{
	Uint32 flags;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
		Log::Out << "Unable to initialize SDL: " << SDL_GetError() << endl;
		return NULL;
	}

	atexit(SDL_Quit);

	flags = SDL_WINDOW_OPENGL;

	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight,
		flags);

	if (!_window) {
		Log::Out << "Unable to create window." << endl;
		checkSDLError(__LINE__);
	}

	SDL_Surface *icon = IMG_Load("data/UWOLIcon.png");
	SDL_SetWindowIcon(_window, icon);

	_mainContext = SDL_GL_CreateContext(_window);

	setGLAttributes();

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	this->_useFramebuffer = initFramebuffer();

	glViewport(0, 0, screenWidth, screenHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);
	glDisable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);

	glDisable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);

	glAlphaFunc(GL_GREATER, 0.01f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(2.0f);

	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_uvBuffer);
	glGenBuffers(1, &_colorBuffer);
	glGenBuffers(1, &_lineVertexBuffer);

	this->ResetMVP();

	return _window;
}

bool GLFuncs::initFramebuffer() {
	// Inicializemos el framebuffer para poder renderizar sobre una textura, así podremos aplicar shaders 
	// a lo que pintemos. ;)
	glGenFramebuffers(1, &_frameBufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);

	glGenTextures(1, &_renderedTexture);
	glBindTexture(GL_TEXTURE_2D, _renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _renderedTexture, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Out << "Unable to initialize Framebuffer! " << gluErrorString(glGetError()) << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);
	return true;
}

void GLFuncs::setGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);		//Use at least 8 bits of Red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);	    //Use at least 8 bits of Green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);		//Use at least 8 bits of Blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		//Use at least 8 bits of Alpha
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	    //Use at least 16 bits for the depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//Enable double buffering

	Log::Out << "OpenGL " << glGetString(GL_VERSION) << " (GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << std::endl;
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

void GLFuncs::BlitColoredRect(int iX, int iY, int width, int height,
	float tx1, float ty1, float tx2, float ty2, float red,
	float green, float blue, float alpha, bool additive)
{
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

	if (additive) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	this->BlitVerts(vertex_buffer_data, sizeof(vertex_buffer_data),
		uv_buffer_data, sizeof(uv_buffer_data),
		color_buffer_data, sizeof(color_buffer_data));
}

void GLFuncs::BlitVerts(float vertex_buffer_data[], unsigned int vBufSize,
	float uv_buffer_data[], unsigned int uvBufSize,
	float color_buffer_data[], unsigned int cBufSize)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vBufSize, vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvBufSize, uv_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, cBufSize, color_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_TEXTURE_2D);

	if (aliasing)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFuncs::DrawPolyLine(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha) {

	vector<GLfloat> coords;
	GLsizei vertexCount = vertexes.size();
	for (VECTOR2 v : vertexes) {
		coords.push_back((float)v.x);
		coords.push_back((float)v.y);
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _lineVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat), &(coords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_LINES, 0, vertexCount);

	glDisableVertexAttribArray(0);
}

GLuint GLFuncs::CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

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
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

GLuint GLFuncs::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);
	glCompileShader(shader);

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
	glDeleteShader(shaderId);
}

void GLFuncs::UseProgram(GLuint programId) {
	glUseProgram(programId);
}

GLuint GLFuncs::GetFramebufferTexture() {
	return this->_renderedTexture;
}

void GLFuncs::SwapBuffers()
{
	if (this->_useFramebuffer) {
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Render on the whole framebuffer, complete from the lower left corner to the upper right
		glViewport(0, 0, _screenWidth, _screenHeight);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Renderizar la textura.
		BlitRect(0, 0, _screenWidth, _screenHeight, 0.0f, 1.0f, 1.0f, 0.0f);
	}

	SDL_GL_SwapWindow(this->_window);

	if (this->_useFramebuffer) {
		// Restore so we draw in the texture again.
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferName);
		glViewport(0, 0, _screenWidth, _screenHeight);
	}
}
