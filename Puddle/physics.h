#pragma once

#include "macros.h"
#include "gameObjects.h"
#include "gameLogic.h"

enum COLLISION_TYPE {
	RECT_RECT_COLLISION = 0,
	CIRCLE_RECT_COLLISION,
	CIRCLE_CIRCLE_COLLISION
};

struct PhysicsSystem {
	PhysicsSystem();

	void update(float deltaT);

	Vec2 calculateGravity();
	void checkCollisions(PuddleApp* app);
	bool checkCollisionWithObject(GameObject* object, ContactInfo* cInfo);

	float getThisFrametime() { return timeSinceLastFrame; }

private:
	float timeSinceLastFrame;

	bool haveCollided(GameObject* first, GameObject* second, ContactInfo* info);
	bool checkCollision(COLLISION_TYPE colType, GameObject* firstObjectToCheck, GameObject* secondObjectToCheck, ContactInfo* info);
};