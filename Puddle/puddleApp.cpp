#include <stdio.h>
#include <string>
#include <sstream>

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
TextTexture* frameTimeText = new TextTexture();

EntityGenerator* entGen = new EntityGenerator();

// DEBUG
extern TextTexture* momentumText;

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
					static Timer timeToStart = Timer();
					timeToStart.start();
					Timer timer = Timer();
					timer.start();

					/*if (Mix_PlayingMusic() == 0) {
						Mix_PlayMusic(music, -1);
					}*/

					SDL_Event e;

					bool runGame = true;

					Timer frameTimer = Timer();
					frameTimer.start();
					float lag = 0.0f;
					float deltaT = frameTimer.getTicks();

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

						lag += frameTimer.getTicks();
						deltaT = frameTimer.getTicks();
						frameTimer.start();

						std::stringstream fpsStr;
						fpsStr.str("");
						fpsStr << "deltaT: " << deltaT;
						frameTimeText->loadFromRenderedText(app->getRenderer()->getRenderer(), fpsStr.str().c_str(), DefaultFont, {0,0,0});

						if (timeToStart.getTicks() > 2000.0f) {
							auto objects = app->getGameObjects();
							objects->at(0)->AddLinearImpulse(Vec2(-14.0f, 2.0f));
							//objects->at(1)->AddAngularImpulse(10.0f);
							timeToStart.stop();
						}
						
						//if (timer.getTicks() / 10000 > 6000.0f) {
						//	app->loadView(debugView);
						//	timer.stop();
						//	timer.start();
						//	timeToStart.start();
						//}

						SDL_SetRenderDrawColor(app->getRenderer()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(app->getRenderer()->getRenderer());
						while (lag >= MS_PER_UPDATE) {
							SDL_SetRenderDrawColor(app->getRenderer()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
							SDL_RenderClear(app->getRenderer()->getRenderer());

							app->updateGame(MS_PER_UPDATE);
							lag -= MS_PER_UPDATE;
						}
						app->setLeftoverTime(lag);
						frameTimeText->render(app->getRenderer()->getRenderer(), 0, 0);
						momentumText->render(app->getRenderer()->getRenderer(), 1000, 0);

						SDL_RenderPresent(app->getRenderer()->getRenderer());
					}
				}

				app->close();
			}
		}
	}

	return 0;
}