#pragma once

#include <vector>

#include <SDL.h>

#include "commonTypedefs.h"
#include "macros.h"
#include "gameLogic.h"
#include "texture.h"
#include "resourceManager.h"
#include "puddleRenderer.h"

extern EntityGenerator* entGen;

struct GameObject {
	Entity _entity;

	GameObject(PhysicsComponent* physics, GraphicsComponent* graphics) : _input(nullptr), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); }
	GameObject(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics) : _input(input), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); }

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