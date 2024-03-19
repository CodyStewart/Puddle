#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <optional>

#include "puddle.h"
#include "puddleRenderer.h"
#include "zipFile.h"
#include "gameObjects.h"
#include "physics.h"

extern SDL_Window* window;
extern PuddleApp* app;
extern TTF_Font* DefaultFont;

PhysicsSystem* physicsSystem = new PhysicsSystem();

PuddleApp::PuddleApp() {
	_resourceManager = new ResourceManager();
	_renderer = new PuddleRenderer();
	_physicsSystem = physicsSystem;
	_gameWorld = new Game();
}

bool PuddleApp::init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		window = SDL_CreateWindow("Puddle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window was not created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_image could not initiailize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}

			if (TTF_Init() == -1) {
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				success = false;
			}

			DefaultFont = TTF_OpenFont("consola.ttf", 36);
			if (DefaultFont == NULL) {
				printf("TTF could not open font! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}
		}
	}

	return success;
}

bool PuddleApp::loadResources() {
	bool success = true;

	ZipFile zip;
	if (!zip.Init(L"assets.zip")) {
		printf("Bad zip file!");
		success = false;
	}
	else {
		success = _resourceManager->loadResources(&zip);
	}
	//// load music
	//music = Mix_LoadMUS("ukulele.mp3");
	//if (music == NULL) {
	//	printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
	//	//success = false;
	//}

	//// load sound effects
	//placeToken = Mix_LoadWAV("pop.wav");
	//if (placeToken == NULL) {
	//	printf("Failed to load place token sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	//	//success = false;
	//}

	//DefaultFont = TTF_OpenFont("arial.ttf", 24);
	//if (DefaultFont == NULL) {
	//	printf("TTF could not open font! SDL_ttf Error: %s\n", TTF_GetError());
	//	success = false;
	//}

	// load textures


	return success;
}

bool PuddleApp::loadView(ViewState viewState) {
	bool success = true;

	// get the current view (we may need this later)
	ViewState previousViewState = _gameWorld->getCurrentView();

	// load the new game view
	success = _gameWorld->generateView(viewState, _resourceManager, _renderer);

	return success;
}

void PuddleApp::addExtraGameObject(GameObject* obj) {
	_gameWorld->addExtraGameObject(obj);
}

ResHandleShrdPtr PuddleApp::getResourceHandle(Resource* res) {
	return _resourceManager->getHandle(res);
}

std::vector<GameObject*>* PuddleApp::getExtraGameObjects() {
	return _gameWorld->getExtraGameObjects();
}

void PuddleApp::updateGame(Uint64 deltaT) {
	_physicsSystem->update(deltaT);
	_gameWorld->update(deltaT);
	//_physicsSystem->checkCollisions(this);
}

void PuddleApp::renderGame() {
	//_renderer->renderGame(_gameObjectManager);
}

PuddleRenderer* PuddleApp::getRenderer() {
	return _renderer;
}

std::vector<GameObject*>* PuddleApp::getGameObjects() {
	return _gameWorld->getGameObjects();
}

void PuddleApp::close() {
	Mix_HaltMusic();

	//Mix_FreeChunk(placeToken);
	//Mix_FreeMusic(music);
	//placeToken = NULL;
	//music = NULL;

	TTF_CloseFont(DefaultFont);
	DefaultFont = NULL;

	_renderer->destroy();

	SDL_DestroyWindow(window);
	window = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}