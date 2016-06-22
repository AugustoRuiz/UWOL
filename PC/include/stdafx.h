// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados

#pragma once


#include <iostream>

// TODO: mencionar aquí los encabezados adicionales que el programa necesita
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

