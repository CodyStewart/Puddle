#pragma once

#include "gameLogic.h"
#include "gameObjects.h"
#include "puddleRenderer.h"

struct PuddleRenderer;

struct PoolBallInputComponent : InputComponent {
	~PoolBallInputComponent() {};
	virtual void update();
};

struct PoolBallPhysicsComponent : PhysicsComponent {
	friend struct PoolBallGraphicsComponent;

	PoolBallPhysicsComponent(SDL_Point pos, float radius);
	~PoolBallPhysicsComponent();
	virtual void update();

private:
	Circle _volume;
	CollisionCircle _collisionVolume;

	void move(PoolBallPhysicsComponent* physics, SDL_Point point);
};

struct PoolBallGraphicsComponent : GraphicsComponent {
	PoolBallGraphicsComponent(PuddleRenderer* renderer, PoolBallPhysicsComponent* physics, Texture* texture);
	~PoolBallGraphicsComponent();
	virtual void update();

private:
	PuddleRenderer* _puddleRenderer;
	PoolBallPhysicsComponent* _physics;
	Texture* _texture;
};