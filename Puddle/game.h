#pragma once

#include "gameObjects.h"
#include "gameLogic.h"
#include "Views.h"

enum ViewState;

struct View;
struct GameObject;

struct Game {
	Game();
	~Game();

	void update(Uint64 deltaT);

	bool generateView(ViewState viewState, ResourceManager* resManager, PuddleRenderer* renderer);

	void addExtraGameObject(GameObject* obj);

	std::vector<GameObject*>* getGameObjects();
	std::vector<GameObject*>* getExtraGameObjects();
	GameObject* getObject(uint32 id);

	ViewState const getCurrentView();

private:
	View* _world;
	ViewState _currentView;
	std::vector<GameObject*> _extraGameObjects;
};