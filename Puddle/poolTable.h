#pragma once

#include "gameLogic.h"
#include "gameObjects.h"
#include "puddleRenderer.h"

/* Defines the Pool Table and Pool Wall colliders
*
*
*/

struct PoolWallInputComponent : InputComponent {
	~PoolWallInputComponent() {};
	virtual void update();
};

struct PoolWallPhysicsComponent : PhysicsComponent {
	friend struct PoolWallGraphicsComponent;

	PoolWallPhysicsComponent(SDL_Point pos, int width, int height);
	~PoolWallPhysicsComponent();
	virtual void update();

private:
	Vec2 _velocity;
	Rect _volume;
	CollisionBox _collisionVolume;

	//void move(PoolWallPhysicsComponent* physics);
};

struct PoolWallGraphicsComponent : GraphicsComponent {
	PoolWallGraphicsComponent(PuddleRenderer* renderer, PoolWallPhysicsComponent* physics, Texture* texture);
	~PoolWallGraphicsComponent();
	virtual void update();

private:
	PuddleRenderer* _puddleRenderer;
	PoolWallPhysicsComponent* _physics;
	Texture* _texture;
};

//struct PoolTable {
//
//};