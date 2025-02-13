#pragma once
#include "GameStateManager.h"
#include "Camera.h"

#include "SDL2/SDL.h"

void renderGraphics(SDL_Renderer* &Renderer, GameStateManager &GameWorld, Camera GameCamera);