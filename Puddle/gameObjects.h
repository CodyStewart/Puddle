#pragma once

#include <vector>

#include <SDL.h>

#include "commonTypedefs.h"
#include "macros.h"
#include "gameLogic.h"
#include "texture.h"
#include "resourceManager.h"
#include "puddleRenderer.h"

struct GameObject {
	virtual void render(SDL_Renderer* renderer);
	virtual void move(SDL_Point point);

};

struct PoolBall : GameObject {
	PoolBall();
	PoolBall(Entity ball, Texture* ballTexture);
	PoolBall(Entity ball, Texture* ballTexture, SDL_Point pos, float radius);
	~PoolBall();

	void move(SDL_Point point) override;

	void render(SDL_Renderer* renderer) override;

private:
	Entity _ball;
	Texture* _ballTexture;
	Circle _shape;
	CollisionCircle _collision;
};



struct GameObjectManager {
	GameObjectManager();

	bool generateGameObjects(ResourceManager* resManager, PuddleRenderer* renderer);

	void addGameObject(GameObject* obj);

	std::vector<GameObject*>* getGameObjects();

private:
	std::vector<GameObject*> _gameObjects;
};