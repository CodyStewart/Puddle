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
	ShapeType _shapeType = UNKNOWN;
	bool _isMovable = false;
	int _mass = 0;

	GameObject() : _entity(Entity()), _input(nullptr), _physics(nullptr), _graphics(nullptr) { forcesList.clear(); impulsesList.clear(); }
	GameObject(PhysicsComponent* physics, GraphicsComponent* graphics) : _input(nullptr), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); _physics->setID(_entity._id);  forcesList.clear(); impulsesList.clear(); }
	GameObject(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics) : _input(input), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); _physics->setID(_entity._id); forcesList.clear(); impulsesList.clear(); }

	void update() {
		_input->update();
		_physics->update(&forcesList, &impulsesList);
		_graphics->update();
	}

	void notifyCollision(GameObject* objectCollided) { _physics->resolveCollision( objectCollided ); }

	void setMovable(bool value) { _isMovable = value; }
	void setShapeType(ShapeType shapeType) { _shapeType = shapeType; }
	void setPosition(Point pos) { _physics->setPosition(pos); }
	void setMass(int mass) { _mass = mass; }
	void setVelocity(Vec2 vel) { _physics->setVelocity(vel); }

	GraphicsComponent* getGraphicsComp() { return _graphics; }
	uint32 getID() { return _entity._id; }
	Vec2 getVelocity() { return _physics->getVelocity(); }
	Vec2 getNormal() { return _physics->getNormal(); }
	Shape getCollider() { return _physics->getCollider(); }
	ColliderData getData() { return _physics->getData(); }
	Point getPosition() { return _physics->getPosition(); }

	void AddForce(Vec2 forceApplied) { forcesList.push_back(forceApplied); }
	void AddImpulse(Vec2 impulseApplied) { impulsesList.push_back(impulseApplied); }

private:
	InputComponent* _input;
	PhysicsComponent* _physics;
	GraphicsComponent* _graphics;

	std::list<Vec2> forcesList;
	std::list<Vec2> impulsesList;
};