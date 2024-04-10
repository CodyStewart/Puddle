#pragma once

#include <iostream>

#include <SDL.h>

#include "puddle.h"

// raw data for an image
struct RawImageData {
	char* buffer = nullptr;	// pixel data buffer
	int width = 0;	// width of the image
	int height = 0;	// height of the image
	int size = 0;	// size of the image. This is width * height * (depth * 8)
	int depth = 0;	// depth of each pixel. Measured as number of 8-bit components (thus RBGA = 4)
};

struct PuddleRenderer {
	PuddleRenderer();
	
	bool init(SDL_Window* window);
	void destroy();

	void setLeftoverTime(float leftoverTime);
	//void renderGame(GameObjectManager* GOManager);

	SDL_Renderer* getRenderer();
	float getLeftoverTime() { return _leftoverTime; }

private:
	SDL_Renderer* _renderer;
	float _leftoverTime = 0.0f;
	
	//void renderGameEntities(GameObjectManager* GOManager);
};
