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

	//Log::Out << "TextureMgr: Loading texture '" << sFileName << "'...";

	if (cache.find(sFileName) != cache.end())
	{
		//Log::Out << "IN CACHE!" << endl;
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
				//Log::Out << "texture #" << result->texture << endl;
				result->width = textureSurf->w;
				result->height = textureSurf->h;
				cache[sFileName] = result;

				Log::Out << "Texture: '" << sFileName << "'" << endl;
				dumpTextureInfo(result->texture);
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
		//Log::Out << "TextureMgr: Deleting texture #" << iter->second->texture << "." << endl;
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
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureSurf->w, textureSurf->h, 0, textureFormat, GL_UNSIGNED_BYTE, textureSurf->pixels);

		//Use nearest filtering, very good
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		result = new TEXTUREINFO();
		result->texture = texNumber;
	}
	return result;
}

void TextureMgr::dumpTextureInfo(GLuint texNumber) {
	GLint value;
	Log::Out << "Texture info:" << endl << "-------------" << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &value);
	Log::Out << "Size: (" << value << ", ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &value);
	Log::Out << value << ") ; depth: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, &value);
	Log::Out << value << endl << "Internal format: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &value);
	Log::Out << getInternalFormatString(value) << endl << "Red size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &value);
	Log::Out << value << endl << "Green size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, &value);
	Log::Out << value << endl << "Blue size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, &value);
	Log::Out << value << endl << "Alpha size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &value);
	Log::Out << value << endl << "Depth size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH_SIZE, &value);
	Log::Out << value << endl << "Compressed: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &value);
	Log::Out << value << endl << "Compressed image size: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &value);
	Log::Out << value << endl << "Buffer offset: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BUFFER_OFFSET, &value);
	Log::Out << value << endl << "-------------" << endl;
}

string TextureMgr::getInternalFormatString(GLint value) {
	stringstream ss;

	switch (value) {
		case 0x1907:
			ss << "GL_RGB"; break;
		case 0x1908:
			ss << "GL_RGBA"; break;
		case 0x80E0:
			ss << "GL_BGR"; break;
		case 0x80E1:
			ss << "GL_BGRA"; break;
		case 0x8032:
			ss << "GL_UNSIGNED_BYTE_3_3_2"; break;
		case 0x8362:
			ss << "GL_UNSIGNED_BYTE_2_3_3_REV"; break;
		case 0x8363:
			ss << "GL_UNSIGNED_SHORT_5_6_5"; break;
		case 0x8364:
			ss << "GL_UNSIGNED_SHORT_5_6_5_REV"; break;
		case 0x8033:
			ss << "GL_UNSIGNED_SHORT_4_4_4_4"; break;
		case 0x8365:
			ss << "GL_UNSIGNED_SHORT_4_4_4_4_REV"; break;
		case 0x8034:
			ss << "GL_UNSIGNED_SHORT_5_5_5_1"; break;
		case 0x8366:
			ss << "GL_UNSIGNED_SHORT_1_5_5_5_REV"; break;
		case 0x8035:
			ss << "GL_UNSIGNED_INT_8_8_8_8"; break;
		case 0x8367:
			ss << "GL_UNSIGNED_INT_8_8_8_8_REV"; break;
		case 0x8036:
			ss << "GL_UNSIGNED_INT_10_10_10_2"; break;
		case 0x8368:
			ss << "GL_UNSIGNED_INT_2_10_10_10_REV"; break;
		default:
			ss << "Unknown";
	}
	ss << " (" << value << ")";
	return ss.str();
}
