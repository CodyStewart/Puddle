#include <sstream>

#include "poolBall.h"
#include "physics.h"

#define BOUNCE 0.5f

extern PhysicsSystem* physicsSystem;
extern PuddleApp* app;

void PoolBallInputComponent::update() {

}

PoolBallPhysicsComponent::PoolBallPhysicsComponent(Point pos, float radius) {
	_velocity = Vec2();
	_acceleration = Vec2();
	_volume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_collisionVolume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_prevPosition = pos;
}

PoolBallPhysicsComponent::~PoolBallPhysicsComponent() {
	
}

void PoolBallPhysicsComponent::setPosition(Point pos) {
	_volume._position = pos;
	_collisionVolume._position = pos;
}

void PoolBallPhysicsComponent::setVelocity(Vec2 newVel) {
	this->_velocity._x = newVel._x;
	this->_velocity._y = newVel._y;
	this->_velocity.calculateMagnitude();
}

float findAngleBetween(Vec2 vec1, Vec2 vec2) {
	float dotVector = vec1.dot(vec2);
	float magnitudeProduct = vec1._length * vec2._length;
	float angle = acosf(dotVector / magnitudeProduct);
	return angle;
}

void PoolBallPhysicsComponent::resolveCollision(GameObject* objectCollided) {
	// move back to previous position
	//_volume._position = _prevPosition;
	//_collisionVolume._position = _prevPosition;

	//static int count = 0;
	//count++;
	//printf("count: %d\n", count);

	if (objectCollided->_mass == 0) {	// collision is with wall
		// find the closest point on the wall and reposition the ball
		// closest point on collision box
		float cX, cY;

		// find closest x offset
		if (this->getPosition().x < objectCollided->getPosition().x)
			cX = objectCollided->getPosition().x;
		else if (this->getPosition().x > objectCollided->getPosition().x + objectCollided->getData().width)
			cX = objectCollided->getPosition().x + objectCollided->getData().width;
		else
			cX = this->getPosition().x;

		// find closest y offset
		if (this->getPosition().y < objectCollided->getPosition().y)
			cY = objectCollided->getPosition().y;
		else if (this->getPosition().y > objectCollided->getPosition().y + objectCollided->getData().height)
			cY = objectCollided->getPosition().y + objectCollided->getData().height;
		else
			cY = this->getPosition().y;

		// reposition ball
		Vec2 vec = Vec2(float(cX), float(cY));
		Vec2 vec2 = Vec2(this->getPosition().x, this->getPosition().y);
		Vec2 distanceToCPOC = Vec2(float(cX), float(cY)) - Vec2(this->getPosition().x, this->getPosition().y);
		Vec2 vec3 = distanceToCPOC;
		Vec2 radiusVec = vec3.normalize() * (float)this->getData().radius;
		Vec2 offset = distanceToCPOC - radiusVec;
		Point point = { this->getPosition().x + offset._x, this->getPosition().y + offset._y };
		this->setPosition(point);

		// find the angle between us and the normal given
		Vec2 objectNormal = objectCollided->getNormal();
		if (objectNormal._x == 1.0f) {
			Vec2 vec = this->getVelocity();
			vec._x = abs(vec._x);
			this->setVelocity(vec);
		}
		else if (objectNormal._x == -1.0f) {
			Vec2 vec = this->getVelocity();
			vec._x = -(vec._x);
			this->setVelocity(vec);
		}
		else if (objectNormal._y == 1.0f) {
			Vec2 vec = this->getVelocity();
			vec._y = abs(vec._y);
			this->setVelocity(vec);
		}
		else if (objectNormal._y == -1.0f) {
			Vec2 vec = this->getVelocity();
			vec._y = -(vec._y);
			this->setVelocity(vec);
		}
	}
	else {	// collision is with movable object
		Vec2 firstNormal;
		Vec2 secondNormal;

		// calculate the normals for both the circles
		if (true) {	// if our velocity is not zero
			// find the point of contact between the balls
			if (this->getVelocity()._length > 9)	// DEBUG
				int breakOnthis = 0;
			Point pos = objectCollided->getPosition();
			Vec2 distanceToObject = Vec2(pos.x, pos.y) - Vec2(this->getPosition().x, this->getPosition().y);
			Vec2 vec3 = distanceToObject;
			Vec2 ourRadiusVec = vec3.normalize() * (float)this->getData().radius;
			Vec2 theirRadiusVec = -vec3 * (float)objectCollided->getData().radius;
			
			// DEBUG
			SDL_Renderer* renderer = app->getRenderer()->getRenderer();
			GameObject* thisObject = app->getObject(_id);
			int size = _volume._radius * 2;
			Texture* texture = thisObject->getGraphicsComp()->getTexture();
			texture->renderScaled(renderer, _volume._position.x - _volume._radius, _volume._position.y - _volume._radius, size, size);
			SDL_RenderPresent(renderer);

			// reposition the ball

			Vec2 cpoc = distanceToObject + theirRadiusVec - ourRadiusVec;
			Point offset = { this->getPosition().x + cpoc._x, this->getPosition().y + cpoc._y };
			this->setPosition(offset);
			//texture->renderScaled(renderer, _volume._position.x - _volume._radius, _volume._position.y - _volume._radius, size, size);
			//SDL_RenderPresent(renderer);

			// apply impulse to us
			//float angle = findAngleBetween(this->getVelocity(), distanceToObject);
			ourRadiusVec.normalize();
			Vec2 impulse = Vec2();
			Vec2 ourVelocity = this->getVelocity();
			impulse._x = ourRadiusVec._x * abs(ourVelocity._x);
			impulse._y = ourRadiusVec._y * abs(ourVelocity._y);
			/*if (ourVelocity._x >= 0)
				impulse._x = -abs(impulse._x);
			else
				impulse._x = abs(impulse._x);
			if (ourVelocity._y >= 0)
				impulse._y = -abs(impulse._y);
			else
				impulse._y = abs(impulse._y);*/

			impulse.calculateMagnitude();
			objectCollided->AddImpulse(impulse);
			//printf("Impulse: x=%f, y=%f, l=%f\n", impulse._x, impulse._y, impulse._length);
			impulse = -impulse;
			//printf("Impulse: x=%f, y=%f, l=%f\n", impulse._x, impulse._y, impulse._length);
			_velocity += impulse;
			_velocity.calculateMagnitude();


			//impulse *= 0.6f;
			//impulse.calculateMagnitude();
			//_velocity += impulse;
			//_velocity.calculateMagnitude();

			//if (impulse._length > 0.3f) {
			//	impulse *= 1.2f;
			//	impulse.calculateMagnitude();
			//}

			// apply impulse to them
			//Vec2 reverseImpulse = -impulse;
			//objectCollided->AddImpulse(reverseImpulse);

			//SDL_Point firstPosition = this->getPosition();
			//SDL_Point secondPosition = objectCollided->getPosition();
			//firstNormal = { static_cast<float>(secondPosition.x - firstPosition.x), static_cast<float>(secondPosition.y - firstPosition.y) };
			//secondNormal = { static_cast<float>(firstPosition.x - secondPosition.x), static_cast<float>(firstPosition.y - secondPosition.y) };
			//// find the angle between us and the normal given
			//float angle = findAngleBetween(_velocity, secondNormal);
			//// set the velocity based on the angle
			////SDL_assert(BOUNCE <= 0.5f);
			//objectCollided->AddImpulse(_velocity * (1 - BOUNCE));
			//Vec2 newVelocity = Vec2();
			//Vec2 objectCollidedVelocity = objectCollided->getVelocity();
			//newVelocity._x = BOUNCE * _velocity._x * cosf(angle) + objectCollidedVelocity._x * cosf(angle);
			//newVelocity._y = BOUNCE * _velocity._y * sinf(angle) + objectCollidedVelocity._y * sinf(angle);
			//_velocity._x = newVelocity._x;
			//_velocity._y = newVelocity._y;
			//_velocity.calculateMagnitude();
		}
		else {

		}
	}
}

void PoolBallPhysicsComponent::move(double frametime) {
	if (this->getVelocity()._length > 0.0f) {

		double t = double(UNIT_SIZE) * frametime / double(1000);
		//printf("frametime: %f\n", frametime);
		Vec2 vel = this->getVelocity();
		float pixelsToMoveAlongX = vel._x * t;
		float pixelsToMoveAlongY = vel._y * t;

		if (pixelsToMoveAlongX > -0.03f && pixelsToMoveAlongX < 0.03f && pixelsToMoveAlongX != 0.0f)
			pixelsToMoveAlongX = 0.0f;
		if (pixelsToMoveAlongY > -0.03f && pixelsToMoveAlongY < 0.03f && pixelsToMoveAlongY != 0.0f)
			pixelsToMoveAlongY = 0.0f;

		this->_volume._position.x += pixelsToMoveAlongX;
		this->_volume._position.y += pixelsToMoveAlongY;
		this->_collisionVolume._position = this->_volume._position;
	}

	//physics->_volume._position.x += static_cast<int>(_velocity._x);
	//physics->_volume._position.y += static_cast<int>(_velocity._y);
	//physics->_collisionVolume._position = physics->_volume._position;
}

void PoolBallPhysicsComponent::update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList) {
	//Vec2 gravity = physicsSystem->calculateGravity();
	//_acceleration = gravity;
	double frametime = physicsSystem->getThisFrametime() / 10000.0f;

	Vec2 sumOfForces = Vec2();
	for (auto itr = forcesList->begin(); itr != forcesList->end(); itr++) {
		sumOfForces += *itr;
	}
	for (auto itr = impulsesList->begin(); itr != impulsesList->end(); itr++) {
		sumOfForces += *itr;
	}
	impulsesList->clear();
	//if (sumOfForces._x > 0.0f && sumOfForces._x < 0.1f)
	//	sumOfForces._x = 0.2f;
	//if (sumOfForces._y > 0.0f && sumOfForces._y < 0.1f)
	//	sumOfForces._y = 0.2f;
	//if (sumOfForces._x < 0.0f && sumOfForces._x > -0.1f)
	//	sumOfForces._x = -0.2f;
	//if (sumOfForces._y < 0.0f && sumOfForces._y > -0.1f)
	//	sumOfForces._y = -0.2f;

	_velocity += sumOfForces;
	_prevPosition = _volume._position;
	
	ContactInfo cInfo = ContactInfo();
	GameObject* thisObject = app->getObject(this->_id);
	physicsSystem->checkCollisionWithObject(thisObject, &cInfo);

	//move(frametime);
}

PoolBallGraphicsComponent::PoolBallGraphicsComponent(PuddleRenderer* renderer, PoolBallPhysicsComponent* physics, Texture* texture) {
	_puddleRenderer = renderer;
	_physics = physics;
	_texture = texture;
}

PoolBallGraphicsComponent::~PoolBallGraphicsComponent() {
	if (_texture) {
		delete _texture;
		_texture = nullptr;
	}
}

void PoolBallGraphicsComponent::update() {
	int size = _physics->_volume._radius * 2;
	_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._position.x - _physics->_volume._radius, _physics->_volume._position.y - _physics->_volume._radius, size, size);

	// DEBUG
	//SDL_SetRenderDrawColor(_puddleRenderer->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x, 0, _physics->_volume._position.x, 1050);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y, 1900, _physics->_volume._position.y);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y + _physics->_volume._radius, 1900, _physics->_volume._position.y + _physics->_volume._radius);
}