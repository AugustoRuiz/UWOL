#include "Graphics/TextureMgr.h"
#include "math.h"

TextureMgr::TextureMgr(void) {
}

TextureMgr::~TextureMgr(void) {
}

TextureMgr* TextureMgr::GetInstance() {
	return &_instance;
}

TEXTUREINFO* TextureMgr::LoadTexture(const string &sFileName) {
	TEXTUREINFO* result = NULL;

	Log::Out << "TextureMgr: Loading texture '" << sFileName << "'...";

	if (cache.find(sFileName) != cache.end())
	{
		Log::Out << "IN CACHE!" << endl;
		result = cache[sFileName];
	}
	else
	{
		SDL_Surface *textureSurf = Pack::GetInstance()->GetImg(sFileName);

		if (textureSurf != NULL) {
			int originalWidth = textureSurf->w;
			int originalHeight = textureSurf->h;

			Log::Out << "Image size is (" << originalWidth << "x" << originalHeight << ")" << endl;
#ifndef __APPLE__
			if (!GLEW_ARB_texture_non_power_of_two) {
#endif
				int w = (int) pow(2, ceil(log(textureSurf->w) / log(2))); // Round up to the nearest power of two
				int h = (int) pow(2, ceil(log(textureSurf->h) / log(2)));
				if (w != originalWidth || h != originalHeight) {
					SDL_Surface* newSurface = SDL_CreateRGBSurface(0, w, h, textureSurf->format->BitsPerPixel, textureSurf->format->Rmask, textureSurf->format->Gmask, textureSurf->format->Bmask, textureSurf->format->Amask);
					SDL_BlitSurface(textureSurf, 0, newSurface, 0); // Blit onto a purely RGB Surface
					SDL_FreeSurface(textureSurf);
					textureSurf = newSurface;
				}
#ifndef __APPLE__
			}
#endif
			Log::Out << "Texture size should be (" << textureSurf->w << "x" << textureSurf->h << ")" << endl;
			result = this->GL_LoadTexture(textureSurf);

			if (result != NULL) {
				Log::Out << "texture #" << result->texture << endl;

				result->width = originalWidth;
				result->height = originalHeight;

				result->texCoords.tx1 = result->texCoords.ty1 = 0.0f;
				result->texCoords.tx2 = ((float)originalWidth) / textureSurf->w;
				result->texCoords.ty2 = ((float)originalHeight) / textureSurf->h;

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

void TextureMgr::DeleteTextures()
{
	TEXTUREMAP::iterator iter;

	iter = cache.begin();
	while (iter != cache.end()) {
		//Log::Out << "TextureMgr: Deleting texture #" << iter->second->texture << "." << endl;
		glDeleteTextures(1, &iter->second->texture);
		iter++;
	}
	cache.clear();
}

TEXTUREINFO* TextureMgr::GL_LoadTexture(SDL_Surface *textureSurf) {
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
		Log::Out << (int)textureSurf->format->BytesPerPixel << " bytes per pixel.";
		if (textureSurf->format->BitsPerPixel == 24) {
			if (textureSurf->format->Rmask == 0xff) {
				Log::Out << "Texture format is GL_RGB - ";
				textureFormat = GL_RGB;
			}
			else {
				Log::Out << "Texture format is GL_BGR - ";
				textureFormat = GL_BGR;
			}
		}
		else {
			if (textureSurf->format->Rmask == 0xff) {
				Log::Out << "Texture format is GL_RGBA - ";
				textureFormat = GL_RGBA;
			}
			else {
				Log::Out << "Texture format is GL_BGRA - ";
				textureFormat = GL_BGRA;
			}
		}

		//Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSurf->w, textureSurf->h, 0, textureFormat, GL_UNSIGNED_BYTE, textureSurf->pixels);

		//Use nearest filtering, very good
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		result = new TEXTUREINFO();
		result->texture = texNumber;
	}
	return result;
}

void TextureMgr::dumpTextureInfo(GLuint texNumber) {
	GLint value;
 	glBindTexture(GL_TEXTURE_2D, texNumber);

	Log::Out << "Texture info:" << endl << "-------------" << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &value);
	Log::Out << "Size: (" << value << ", ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &value);
	Log::Out << value << ") ;";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, &value);
	Log::Out << " depth: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &value);
	Log::Out << "Internal format: " << getInternalFormatString(value) << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &value);
	Log::Out << "Red size: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, &value);
	Log::Out << "Green size: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, &value);
	Log::Out << "Blue size: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &value);
	Log::Out << "Alpha size: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH_SIZE, &value);
	Log::Out << "Depth size: " << value << endl;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &value);
	Log::Out << "Compressed: " << value << endl;
//	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &value);
//	Log::Out  << "Compressed image size: " << value << endl;
#ifndef __APPLE__
	Log::Out << "Buffer offset: ";
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BUFFER_OFFSET, &value);
	Log::Out << value << endl;
#endif
	Log::Out << "-------------" << endl;

	glBindTexture(GL_TEXTURE_2D, 0);
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
