#pragma once
#pragma warning (disable:4244)

#include <vector>

#include "gameObjects.h"
#include "resourceManager.h"
#include "puddleRenderer.h"
#include "game.h"
#include "Views.h"

const int DEFAULT_SCREEN_WIDTH = 1920;
const int DEFAULT_SCREEN_HEIGHT = 1080;
const int CURRENT_SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
const int CURRENT_SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;

struct GameObject;
struct PuddleRenderer;
struct PhysicsSystem;
struct View;
struct Game;

enum ViewState;

struct PuddleApp {
	PuddleApp();
	bool init();
	bool loadResources();
	bool loadView(ViewState viewState);

	ResHandleShrdPtr getResourceHandle(Resource* res);
	std::vector<GameObject*>* getExtraGameObjects();

	void addExtraGameObject(GameObject* obj);

	void updateGame(Uint64 deltaT);
	void renderGame();

	PuddleRenderer* getRenderer();
	std::vector<GameObject*>* getGameObjects();
	GameObject* getObject(uint32 id);
	//uint32 getThisFrametime() { return _physicsSystem->getThisFrametime(); }

	void close();

private:
	Game* _gameWorld;
	ResourceManager* _resourceManager;
	//GameObjectManager* _gameObjectManager;
	PuddleRenderer* _renderer;
	PhysicsSystem* _physicsSystem;
};