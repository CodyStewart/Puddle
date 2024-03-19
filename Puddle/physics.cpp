#include "physics.h"

extern PuddleApp* app;

PhysicsSystem::PhysicsSystem() {
	timeSinceLastFrame = 0;
}

void PhysicsSystem::update(Uint64 deltaT) {		// deltaT is stored as 1
	timeSinceLastFrame = deltaT;
}

Vec2 PhysicsSystem::calculateGravity() {
	float amountToAccelerate = GRAVITY * (timeSinceLastFrame / 1000.0f);
	Vec2 gravityVec(0.0f, amountToAccelerate);
	return gravityVec;
}

enum COLLISION_TYPE {
	RECT_RECT_COLLISION = 0,
	CIRCLE_RECT_COLLISION,
	CIRCLE_CIRCLE_COLLISION
};

double distanceSquared(float x1, float y1, float x2, float y2)
{
	float deltaX = x2 - x1;
	float deltaY = y2 - y1;
	double value = deltaX * deltaX + deltaY * deltaY;
	return value;
}

bool checkCollision(COLLISION_TYPE colType, GameObject* firstObjectToCheck, GameObject* secondObjectToCheck) {
	Shape firstCollider = firstObjectToCheck->getCollider();
	Shape secondCollider = secondObjectToCheck->getCollider();
	ColliderData firstObjectData = firstObjectToCheck->getData();
	ColliderData secondObjectData = secondObjectToCheck->getData();

	switch (colType) {
	case RECT_RECT_COLLISION:

		break;
	case CIRCLE_RECT_COLLISION:

		// closest point on collision box
		float cX, cY;

		// find closest x offset
		if (firstCollider._position.x < secondCollider._position.x)
			cX = secondCollider._position.x;
		else if (firstCollider._position.x > secondCollider._position.x + secondObjectData.width)
			cX = secondCollider._position.x + secondObjectData.width;
		else
			cX = firstCollider._position.x;

		// find closest y offset
		if (firstCollider._position.y < secondCollider._position.y)
			cY = secondCollider._position.y;
		else if (firstCollider._position.y > secondCollider._position.y + secondObjectData.height)
			cY = secondCollider._position.y + secondObjectData.height;
		else
			cY = firstCollider._position.y;

		// if the closest point is inside the circle
		if (distanceSquared(firstCollider._position.x, firstCollider._position.y, cX, cY) < firstObjectData.radius * firstObjectData.radius)
		{
			// this box and the circle have collided
			return true;
		}
		
		// no collision
		return false;

		break;
	case CIRCLE_CIRCLE_COLLISION:
		
		//Calculate total radius squared
		int totalRadiusSquared;
		totalRadiusSquared = firstObjectData.radius + secondObjectData.radius;
		totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

		// if the distance between the centers of the circles is less than the sum of their radii
		if (distanceSquared(firstCollider._position.x, firstCollider._position.y, secondCollider._position.x, secondCollider._position.y) < (totalRadiusSquared))
		{
			// the circles have collided
			return true;
		}

		return false;

		break;
	default:
		printf("Unknown collision type in checkCollision!\n");
		break;
	}

	return false;
}

bool haveCollided(GameObject* first, GameObject* second, ContactInfo* info) {
	bool hadCollision = false;

	if (first->_shapeType == CIRCLE && second->_shapeType == CIRCLE) {
		bool collision = checkCollision(CIRCLE_CIRCLE_COLLISION, first, second);
		if (collision) {
			first->notifyCollision(second);
			second->notifyCollision(first);

			hadCollision = true;
		}
	}
	else if (first->_shapeType == CIRCLE && second->_shapeType == RECT) {
		bool collision = checkCollision(CIRCLE_RECT_COLLISION, first, second);
		if (collision) {
			// tell the objects that a collision has occurred
			first->notifyCollision(second);
			//second->notifyCollision(first);
		}
	}
	else if (first->_shapeType == RECT && second->_shapeType == CIRCLE) {
		bool collision = checkCollision(CIRCLE_RECT_COLLISION, second, first);
		if (collision) {
			first->notifyCollision(second);
			//second->notifyCollision(first);
		}
	}
	else if (first->_shapeType == RECT && second->_shapeType == RECT) {
		bool collision = checkCollision(RECT_RECT_COLLISION, first, second);
		if (collision) {
			first->notifyCollision(second);
			//second->notifyCollision(first);
		}
	}
	return hadCollision;
}

//void PhysicsSystem::checkCollisions(PuddleApp* app) {
//	std::vector<GameObject*>* gameObjects = app->getGameObjects();
//	// add friction
//	for (std::vector<GameObject*>::iterator movableObject = gameObjects->begin(); movableObject != gameObjects->end(); movableObject++) {
//		if ((*movableObject)->_shapeType == CIRCLE && (*movableObject)->getVelocity() != Vec2()) {
//			//Vec2 frictionVec = Vec2(1.0f, 1.0f);
//			//frictionVec *= timeSinceLastFrame / 10000000.0f;
//
//			//Vec2 vel = (*movableObject)->getVelocity();
//			//if (vel._x > 0)
//			//	frictionVec._x = -frictionVec._x;
//			//if (vel._y > 0)
//			//	frictionVec._y = -frictionVec._y;
//			//(*movableObject)->AddImpulse(frictionVec);
//			//vel *= 0.999;
//			//(*movableObject)->setVelocity(vel);
//		}
//	}
//
//	for (std::vector<GameObject*>::iterator firstObject = gameObjects->begin(); firstObject != gameObjects->end(); firstObject++) {
//		for (std::vector<GameObject*>::iterator secondObject = firstObject + 1; secondObject != gameObjects->end(); secondObject++) {
//			if (firstObject == secondObject)
//				continue;
//			if (haveCollided(*firstObject, *secondObject)) {
//				// NOTE: Check the total momentum of the balls here
//				float totalMomentum = 0;
//				for (std::vector<GameObject*>::iterator firstObject = gameObjects->begin(); firstObject != gameObjects->end(); firstObject++) {
//					totalMomentum += (*firstObject)->getVelocity()._length;
//				}
//				printf("Total Momemntum: %f\n", totalMomentum);
//			}
//		}
//	}
//}

ContactInfo* checkCollisionWithObject(GameObject* thisObject, ContactInfo* cInfo) {
	std::vector<GameObject*>* gameObjects = app->getGameObjects();

	ContactInfo cInfo = ContactInfo();

	for (std::vector<GameObject*>::iterator otherObject = gameObjects->begin(); otherObject != gameObjects->end(); otherObject++) {
		if (thisObject == *otherObject)
			continue;

		haveCollided(thisObject, *otherObject, cInfo);
	}
}