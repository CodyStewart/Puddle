#pragma once

#include <vector>

#include "gameObjects.h"

struct GameObject;

enum ViewState {
	startView = 0,
	debugView
};

struct View {
	View(ViewState viewState); // default constructor (may use for a special debug purpose?)
	View(ViewState viewState, ResourceManager* resManager, PuddleRenderer* renderer);
	~View();

	void addGameObject(GameObject* obj);

	void update();

private:
	std::vector<GameObject*> _gameObjects;
};