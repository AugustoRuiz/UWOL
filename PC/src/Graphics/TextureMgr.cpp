#include "TextureMgr.h"

TextureMgr::TextureMgr(void)
{
}

TextureMgr::~TextureMgr(void)
{
}

TextureMgr * TextureMgr::GetInstance()
{
	return &_instance;
}

TEXTUREINFO* TextureMgr::LoadTexture(const string &sFileName)
{
	TEXTUREINFO* result = NULL;

	Log::Out << "TextureMgr: Loading texture '" << sFileName << "'...";

	if (cache.find(sFileName) != cache.end())
	{
		Log::Out << "IN CACHE!" << endl;
		result = cache[sFileName];
	}
	else
	{
		SDL_Surface *textureSurf = IMG_Load(sFileName.c_str());

		if (textureSurf != NULL)
		{
			result = this->GL_LoadTexture(textureSurf);

			if (result != NULL)
			{
				Log::Out << "texture #" << result->texture << endl;
				result->width = textureSurf->w;
				result->height = textureSurf->h;
				cache[sFileName] = result;
			}
			else
			{
				Log::Out << "OpenGL couldn't load texture " << sFileName << "!!!" << endl;
			}
			SDL_FreeSurface(textureSurf);
		}
		else
		{
			Log::Out << "SDL couldn't load texture " << sFileName << "!!!" << endl;
		}
	}

	return result;
}

//TEXTUREINFO* TextureMgr::LoadTexture(SDL_RWops *rw)
//{
//	TEXTUREINFO* result = NULL;
//
//	SDL_Surface *textureSurf = IMG_Load_RW(rw, 0);
//
//	if(textureSurf != NULL)
//	{
//		result = this->GL_LoadTexture(textureSurf);
//
//		if(result != NULL)
//		{
//		    result->width = textureSurf->w;
//		    result->height = textureSurf->h;
//		}
//		SDL_FreeSurface(textureSurf);
//	}
//
//	return result;
//}

void TextureMgr::DeleteTextures()
{
	TEXTUREMAP::iterator iter;

	iter = cache.begin();
	while (iter != cache.end())
	{
		Log::Out << "TextureMgr: Deleting texture #" << iter->second->texture << "." << endl;
		glDeleteTextures(1, &iter->second->texture);
		iter++;
	}
	cache.clear();
}

TEXTUREINFO* TextureMgr::GL_LoadTexture(SDL_Surface *textureSurf)
{
	GLuint texNumber = 0;
	TEXTUREINFO *result = NULL;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//Create the texture
	glGenTextures(1, &texNumber);

	if (texNumber == 0) {
		GLenum err = glGetError();
		Log::Out << "Error loading texture: ERROR " << err << endl;
	}
	else {
		//Load the texture
		glBindTexture(GL_TEXTURE_2D, texNumber);
		GLenum textureFormat;

		if (textureSurf->format->BitsPerPixel == 24) {
			if (textureSurf->format->Rmask == 0xff) {
				textureFormat = GL_RGB;
			}
			else {
				textureFormat = GL_BGR;
			}
		}
		else {
			if (textureSurf->format->Rmask == 0xff) {
				textureFormat = GL_RGBA;
			}
			else {
				textureFormat = GL_BGRA;
			}
		}

		//Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, textureSurf->format->BytesPerPixel, textureSurf->w, textureSurf->h, 0, textureFormat, GL_UNSIGNED_BYTE, textureSurf->pixels);

		//Use nearest filtering, very good
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		result = new TEXTUREINFO();
		result->texture = texNumber;
	}
	return result;
}