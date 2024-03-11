#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "puddle.h"
#include "puddleRenderer.h"
#include "texture.h"
#include "gameLogic.h"

SDL_Window* window = nullptr;

PuddleApp* app = new PuddleApp();

TTF_Font* DefaultFont = nullptr;

//PuddleRenderer* appRenderer = new PuddleRenderer();

EntityGenerator* entGen = new EntityGenerator();

Texture* circleTexture = new Texture();

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
			//circleTexture->loadTextureFromFile(appRenderer->getRenderer(), "blackOutlineCircle.png");
			if (!app->loadResources()) {
				printf("Failed to load media!\n");
			}
			else {
				if (!app->loadObjects()) {
					printf("Failed to load game objects!\n");
				}
				else {

				}
				/*if (Mix_PlayingMusic() == 0) {
					Mix_PlayMusic(music, -1);
				}*/

				SDL_Event e;

				bool runGame = true;

				//Resource circleText("blackOutlineCircle.png");
				//ResHandleShrdPtr circleHandle = app->getResourceHandle(&circleText);
				//Texture ballTexture(appRenderer->getRenderer(), circleHandle.get()->buffer(), circleHandle.get()->size());
				////if (circleHandle.get()) {
				//	
				//	Entity oolBall = entGen->generatateNewEntity();
				//	SDL_Point point = { 50, 50 };
				//	PoolBall oneBall(oolBall, &ballTexture, point);
					//oneBall.render(appRenderer->getRenderer());
					//app->addGameObject(&oneBall);
				//}
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
					SDL_SetRenderDrawColor(app->getRenderer()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(app->getRenderer()->getRenderer());
					app->updateGame();
					//circleTexture->render(this->getRenderer(), 100, 100);
					//renderGameEntities(app);
					//oneBall.render(appRenderer->getRenderer());
					//circleTexture->render(appRenderer->getRenderer(), 400, 400);
					//app->renderGame();
					SDL_RenderPresent(app->getRenderer()->getRenderer());
					//appRenderer->renderGame(app);
				}

				app->close();
			}
		}
	}

	return 0;
}