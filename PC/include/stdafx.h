// stdafx.h: archivo de inclusi�n de los archivos de inclusi�n est�ndar del sistema,
// o archivos de inclusi�n espec�ficos de un proyecto utilizados frecuentemente,
// pero rara vez modificados

#pragma once


#include <iostream>

// TODO: mencionar aqu� los encabezados adicionales que el programa necesita
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

