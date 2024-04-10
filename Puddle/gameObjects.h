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
	float _InvMass = 0.0f;

	GameObject() : _entity(Entity()), _input(nullptr), _physics(nullptr), _graphics(nullptr) { forcesList.clear(); impulsesList.clear(); angularImpulsesList.clear(); }
	GameObject(PhysicsComponent* physics, GraphicsComponent* graphics) : _input(nullptr), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); _physics->setID(_entity._id);  forcesList.clear(); impulsesList.clear(); angularImpulsesList.clear(); }
	GameObject(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics) : _input(input), _physics(physics), _graphics(graphics) { _entity = entGen->generatateNewEntity(); _physics->setID(_entity._id); forcesList.clear(); impulsesList.clear(); angularImpulsesList.clear(); }

	void update(float deltaT) {
		_input->update();
		_physics->update(&forcesList, &impulsesList, &angularImpulsesList);
		_graphics->update(deltaT);
	}

	//void notifyCollision(GameObject* objectCollided) { _physics->resolveCollision( objectCollided ); }

	void setMovable(bool value) { _isMovable = value; }
	void setShapeType(ShapeType shapeType) { _shapeType = shapeType; }
	void setPosition(Point pos) { _physics->setPosition(pos); }
	void setMass(float mass) { mass > 10000.0f ? _InvMass = 0 : _InvMass = 1.0f / mass; }
	void setVelocity(Vec2 vel) { _physics->setLinearVelocity(vel); }
	void setElasticity(float restitution) { _physics->setElasticity(restitution); }

	GraphicsComponent* getGraphicsComp() { return _graphics; }
	uint32 getID() { return _entity._id; }
	Vec2 getLinearVelocity() { return _physics->getLinearVelocity(); }
	Vec2 getNormal() { return _physics->getNormal(); }
	Shape getCollider() { return _physics->getCollider(); }
	ColliderData getData() { return _physics->getData(); }
	Point getPosition() { return _physics->getPosition(); }
	float getElasticity() { return _physics->getElasticity(); }

	void AddLinearForce(Vec2 forceApplied) { forcesList.push_back(forceApplied); }
	void AddLinearImpulse(Vec2 impulseApplied) { impulsesList.push_back(impulseApplied); }
	void AddAngularImpulse(const float impulse) { angularImpulsesList.push_back(impulse); }

private:
	InputComponent* _input;
	PhysicsComponent* _physics;
	GraphicsComponent* _graphics;

	std::list<Vec2> forcesList;
	std::list<Vec2> impulsesList;
	std::list<float> angularImpulsesList;
};