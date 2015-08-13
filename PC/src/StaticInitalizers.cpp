#pragma once

#include "Graphics.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "Game.h"

TextureMgr TextureMgr::_instance;
GLFuncs GLFuncs::_instance;
Graphics Graphics::_instance;

InputManager InputManager::_instance;

MusicManager MusicManager::_instance;

Game Game::_instance;
