#include "GLFuncs.h"

GLFuncs *GLFuncs::GetInstance()
{
	return &_instance;
}

SDL_Surface *GLFuncs::Initialize(int width, int height, GLboolean fullscreen)
{
	SDL_Surface *screen;
	Uint32 flags;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		Log::Out << "Unable to initialize SDL: " << SDL_GetError() << endl;
		return NULL;
	}

	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);		//Use at least 8 bits of Red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);	    //Use at least 8 bits of Green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);		//Use at least 8 bits of Blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		//Use at least 8 bits of Alpha
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	    //Use at least 16 bits for the depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//Enable double buffering

	flags = SDL_OPENGLBLIT | SDL_DOUBLEBUF | SDL_HWSURFACE ; // | SDL_ASYNCBLIT | SDL_RESIZABLE 

	if (fullscreen)
	{
		flags |= SDL_FULLSCREEN;
	}

    SDL_Surface *icon = IMG_Load("data/UWOLIcon.png");
    SDL_WM_SetIcon(icon, NULL);

	screen = SDL_SetVideoMode(width, height, 32, flags);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f,0.0f,0.0f,0.5f);
	glClearDepth(1.0f);
	
	glDepthFunc(GL_LEQUAL);	
	glDisable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
	
	glDisable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	
	glAlphaFunc(GL_GREATER,0.01f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(2.0f);

	return screen;
}

void GLFuncs::BlitColoredRect(GLuint texNumber, int iX, int iY, int width, int height,
							  float tx1, float ty1, float tx2, float ty2, float red,
							  float green, float blue, float alpha, bool additive)
{
	glEnable(GL_TEXTURE_2D);
	//Load the texture
	glBindTexture(GL_TEXTURE_2D, texNumber);
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    if(aliasing)
    {
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
    

	glEnable(GL_BLEND);
	
	glColor4f(red, green, blue, alpha);
	
	if(additive)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glBegin(GL_QUADS);
	//Top-left vertex (corner)
	glTexCoord2f(tx1, ty1);
	glVertex2i(iX, iY);

	//Top-right vertex (corner)
	glTexCoord2f(tx2, ty1);
	glVertex2i(iX + width, iY);

	//Bottom-right vertex (corner)
	glTexCoord2f(tx2, ty2);
	glVertex2i(iX + width, iY + height);

	//Bottom-left vertex (corner)
	glTexCoord2f(tx1, ty2);
	glVertex2i(iX, iY + height);
	glEnd();
}


void GLFuncs::BlitRect(GLuint texNumber, int iX, int iY, int width, int height,
					   float tx1, float ty1, float tx2, float ty2)
{
	BlitColoredRect(texNumber, iX, iY, width, height, tx1, ty1, tx2, ty2, 1.0f, 1.0f, 1.0f, 1.0f, false);
}

void GLFuncs::BlitShadow(GLuint texNumber, int iX, int iY, int width, int height,
					     float tx1, float ty1, float tx2, float ty2)
{
	BlitColoredRect(texNumber, iX, iY, width, height, tx1, ty1, tx2, ty2, 0.0f, 0.0f, 0.0f, 0.5f, false);
}

void GLFuncs::BlitRect(GLuint texNumber, int iX, int iY, int width, int height,
					   float tx1, float ty1, float tx2, float ty2, float alpha)
{
	BlitColoredRect(texNumber, iX, iY, width, height, tx1, ty1, tx2, ty2, 1.0f, 1.0f, 1.0f, alpha, false);
}

void GLFuncs::DrawPolyLine(const vector<VECTOR2> &vertexes, float red, float green, float blue, float alpha) {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for (VECTOR2 v : vertexes) {
		glVertex2i(v.x, v.y);
	}
	glEnd();
}
