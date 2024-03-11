#pragma once

#include "macros.h"
#include "gameObjects.h"
#include "gameLogic.h"

struct PhysicsSystem {
	PhysicsSystem();

	void update(Uint32 deltaT);

	Vec2 calculateGravity();

private:
	uint32 timeSinceLastFrame;
};