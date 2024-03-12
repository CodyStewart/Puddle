#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "puddle.h"
#include "puddleRenderer.h"
#include "texture.h"
#include "gameLogic.h"
#include "timer.h"
#include "Views.h"

SDL_Window* window = nullptr;

PuddleApp* app = new PuddleApp();

TTF_Font* DefaultFont = nullptr;

EntityGenerator* entGen = new EntityGenerator();

int main(int argc, char* args[]) {
	if (!app->init()) {
		printf("Failed to initialize app!\n");
	}
	else {
		// initialize the renderer
		if (!app->getRenderer()->init(window)) {
			printf("Failed to initialize renderer!\n");
		}
		else {
			printf("hello!\n");
			// load resources to the resources cache
			if (!app->loadResources()) {
				printf("Failed to load media!\n");
			}
			else {
				if (!app->loadView(debugView)) {
					printf("Failed to load view!\n");
				}
				else {

				}
				/*if (Mix_PlayingMusic() == 0) {
					Mix_PlayMusic(music, -1);
				}*/

				SDL_Event e;

				bool runGame = true;

				Uint32 deltaT = 0;
				Timer frameTimer = Timer();
				frameTimer.start();

				while (runGame) {
					while (SDL_PollEvent(&e) != 0) {
						if (e.type == SDL_QUIT) {
							runGame = false;
						}
						else if (e.type == SDL_KEYDOWN)
						{
							switch (e.key.keysym.sym) {
							case SDLK_ESCAPE:
								runGame = false;
								break;

							default:
								break;
							}
						}
					}

					deltaT = frameTimer.getTicks();
					frameTimer.start();

					SDL_SetRenderDrawColor(app->getRenderer()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(app->getRenderer()->getRenderer());
					app->updateGame(deltaT);
					SDL_RenderPresent(app->getRenderer()->getRenderer());
				}

				app->close();
			}
		}
	}

	return 0;
}