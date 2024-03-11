#include <stdio.h>
#include <vector>

#include <SDL.h>

#include "puddle.h"
#include "puddleRenderer.h"
#include "texture.h"
#include "gameObjects.h"

extern Texture* circleTexture;

PuddleRenderer::PuddleRenderer() {
	_renderer = nullptr;
}

bool PuddleRenderer::init(SDL_Window* window) {
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr) {
		printf("Renderer was not created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		return true;
	}
}

void PuddleRenderer::destroy() {
	SDL_DestroyRenderer(_renderer);
	_renderer = nullptr;
}

void PuddleRenderer::renderGameEntities(GameObjectManager* GOManager) {
	/*for (std::vector<GameObject>::iterator itr = objectsToRender->begin(); itr != objectsToRender->end(); itr++) {
		itr->render(this->getRenderer());
	}*/
}

void PuddleRenderer::renderGame(GameObjectManager* GOManager) {
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);
	//circleTexture->render(this->getRenderer(), 100, 100);
	renderGameEntities(GOManager);
	SDL_RenderPresent(_renderer);
}

SDL_Renderer* PuddleRenderer::getRenderer() {
	return _renderer;
}