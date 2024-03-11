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
	Entity _entity;

	GameObject(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics) : _input(input), _physics(physics), _graphics(graphics) {} ;

	void update() {
		_input->update();
		_physics->update();
		_graphics->update();
	}

private:
	InputComponent* _input;
	PhysicsComponent* _physics;
	GraphicsComponent* _graphics;
};

struct GameObjectManager {
	GameObjectManager();

	bool generateGameObjects(ResourceManager* resManager, PuddleRenderer* renderer);

	void addGameObject(GameObject* obj);

	std::vector<GameObject*>* getGameObjects();

	void update();

private:
	std::vector<GameObject*> _gameObjects;
};