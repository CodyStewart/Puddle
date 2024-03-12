#include "game.h"
#include "poolBall.h"

Game::Game() {
	_currentView = debugView;
	_world = new View(_currentView);
	_extraGameObjects.clear();
}

Game::~Game() {
	if (_world) {
		delete _world;
	}
	for (auto obj : _extraGameObjects) {
		if (obj)
			delete obj;
	}
}

bool Game::generateView(ViewState viewState, ResourceManager* resManager, PuddleRenderer* renderer) {
	bool success = false;

	if (_world)
		delete _world;
	_world = new View(viewState, resManager, renderer);

	if (_world)
		success = true;
	
	if (success)
		printf("View %d successfully loaded\n", viewState);
	else
		printf("View %d was not loaded\n!", viewState);

	return success;
}

ViewState const Game::getCurrentView() {
	return _currentView;
}

void Game::addExtraGameObject(GameObject* obj) {
	_extraGameObjects.push_back(obj);
}

std::vector<GameObject*>* Game::getExtraGameObjects() {
	return &_extraGameObjects;
}

void Game::update() {
	_world->update();

	for (auto object : _extraGameObjects) {
		object->update();
	}
}