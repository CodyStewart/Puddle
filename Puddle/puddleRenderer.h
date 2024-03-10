#pragma once

#include <iostream>

#include <SDL.h>

#include "puddle.h"

struct GameObjectManager;

struct PuddleRenderer {
	PuddleRenderer();
	
	bool init(SDL_Window* window);
	void destroy();

	void renderGame(GameObjectManager* GOManager);

	SDL_Renderer* getRenderer();

private:
	SDL_Renderer* _renderer;
	
	void renderGameEntities(GameObjectManager* GOManager);
};
