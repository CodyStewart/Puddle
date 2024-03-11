#include "physics.h"

PhysicsSystem::PhysicsSystem() {
	timeSinceLastFrame = 0;
}

void PhysicsSystem::update(Uint32 deltaT) {
	timeSinceLastFrame = deltaT;
}

Vec2 PhysicsSystem::calculateGravity() {
	float amountToAccelerate = GRAVITY * (timeSinceLastFrame / 1000.0f);
	Vec2 gravityVec(0.0f, amountToAccelerate);
	return gravityVec;
}