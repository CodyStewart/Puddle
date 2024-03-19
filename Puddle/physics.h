#pragma once

#include "macros.h"
#include "gameObjects.h"
#include "gameLogic.h"

struct PhysicsSystem {
	PhysicsSystem();

	void update(Uint64 deltaT);

	Vec2 calculateGravity();
	void checkCollisions(PuddleApp* app);
	ContactInfo* checkCollisionWithObject(GameObject* object, ContactInfo* cInfo);

	uint64 getThisFrametime() { return timeSinceLastFrame; }

private:
	uint64 timeSinceLastFrame;
};