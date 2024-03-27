#include <sstream>

#include "poolBall.h"
#include "physics.h"

#define BALL_BOUNCE 1.0f	// coefficient of restitution for Balls
#define WALL_BOUNCE 1.0f	// coefficient of restitution for Walls

extern PhysicsSystem* physicsSystem;
extern PuddleApp* app;

void PoolBallInputComponent::update() {

}

PoolBallPhysicsComponent::PoolBallPhysicsComponent(Point pos, float radius, float restitution) {
	_linearVelocity = Vec2();
	_angularVelocity = 0.0f;
	_volume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_collisionVolume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_prevPosition = pos;
	_elasticity = restitution;
	_orientation = Vec3();
}

PoolBallPhysicsComponent::~PoolBallPhysicsComponent() {
	
}

void PoolBallPhysicsComponent::setPosition(Point pos) {
	_volume._position = pos;
	_collisionVolume._position = pos;
}

void PoolBallPhysicsComponent::setOrientation(Vec3 orient) {
	_orientation = orient;
}

void PoolBallPhysicsComponent::setLinearVelocity(Vec2 newVel) {
	this->_linearVelocity._x = newVel._x;
	this->_linearVelocity._y = newVel._y;
	this->_linearVelocity.calculateMagnitude();
}

void PoolBallPhysicsComponent::setAngularVelocity(float newVel) {
	this->_angularVelocity = newVel;
}

float findAngleBetween(Vec2 vec1, Vec2 vec2) {
	float dotVector = vec1.dot(vec2);
	float magnitudeProduct = vec1._length * vec2._length;
	float angle = acosf(dotVector / magnitudeProduct);
	return angle;
}

void PoolBallPhysicsComponent::move(double frametime) {
	if (this->getLinearVelocity()._length > 0.0f) {

		double t = double(UNIT_SIZE) * frametime / double(1000);
		//printf("frametime: %f\n", frametime);
		Vec2 vel = this->getLinearVelocity();
		float pixelsToMoveAlongX = vel._x * t;
		float pixelsToMoveAlongY = vel._y * t;

		//if (pixelsToMoveAlongX > -0.03f && pixelsToMoveAlongX < 0.03f && pixelsToMoveAlongX != 0.0f)
		//	pixelsToMoveAlongX = 0.0f;
		//if (pixelsToMoveAlongY > -0.03f && pixelsToMoveAlongY < 0.03f && pixelsToMoveAlongY != 0.0f)
		//	pixelsToMoveAlongY = 0.0f;

		this->_volume._position.x += pixelsToMoveAlongX;
		this->_volume._position.y += pixelsToMoveAlongY;
		this->_collisionVolume._position = this->_volume._position;

		// we must also change the x and y values of our orientation based on the linear velocity

	}

	if (fabs(this->getAngularVelocity()) > 0.0f) {
		// we only alter the z value of our orientation
		float angVel = this->getAngularVelocity();
		float radius = this->getData().radius;
		float fullRevolution = 2.0f * PI * radius;
		double t = (double)(frametime / double(1000));
		float rotateBy = angVel * UNIT_SIZE * t / fullRevolution;
		_orientation._z += rotateBy;
	}

	//physics->_volume._position.x += static_cast<int>(_velocity._x);
	//physics->_volume._position.y += static_cast<int>(_velocity._y);
	//physics->_collisionVolume._position = physics->_volume._position;
}

void PoolBallPhysicsComponent::resolveCollision(ContactInfo* cInfo) {

	GameObject* objectCollided = cInfo->B;

	const float invMassA = cInfo->A->_InvMass;
	const float invMassB = cInfo->B->_InvMass;

	const float elasticityA = cInfo->A->getElasticity();
	const float elasticityB = cInfo->B->getElasticity();
	const float elasticity = elasticityA * elasticityB;

	// calculate the impulse
	Vec2 n = cInfo->normal;
	Vec2 relativeVel = cInfo->A->getLinearVelocity() - cInfo->B->getLinearVelocity();

	float impulseJNumerator = relativeVel.dot(n) * -(1.0f + elasticity);
	float impulseJDenominator = n.dot(n * (invMassA + invMassB));
	float impulse = impulseJNumerator / impulseJDenominator;
	Vec2 impulseJVecA = n * impulse * invMassA;
	Vec2 impulseJVecB = (n * impulse * invMassB) * -1.0f;

	// apply the linear impulse to us and to the object collided with
	_linearVelocity += impulseJVecA;
	objectCollided->AddLinearImpulse(impulseJVecB);

	// calculate the angular impulse
	Point pos = this->getPosition();
	Vec2 r = (cInfo->ApointWrldSpace - pos.toVec()).getPerp();
	float perpDotOfImpact = impulseJVecA.perpDot(n);
	float radius = this->getData().radius;
	float invTensor = 1.0f / (1.0f * (radius * radius));	// inertia tensor of point mass = Mr^2
	float angImpulseA = impulse * invTensor * perpDotOfImpact; // apply the inverse of the inertia tensor to the angular impulse
	float angImpulseB = angImpulseA * -1.0f;
	_angularVelocity += angImpulseA;	
	objectCollided->AddAngularImpulse(angImpulseB);

	// reposition ourselves
	Vec2 ds = cInfo->BpointWrldSpace - cInfo->ApointWrldSpace;
	const float tA = invMassA / (invMassA + invMassB);
	const float tB = invMassB / (invMassA + invMassB);
	Point APos = cInfo->A->getPosition();
	Point BPos = cInfo->B->getPosition();
	APos += (ds * tA).toPoint();
	BPos -= (ds * tB).toPoint();
	this->setPosition(APos);
	cInfo->B->setPosition(BPos);

	// DEBUG
	//SDL_Renderer* renderer = app->getRenderer()->getRenderer();
	//GameObject* thisObject = app->getObject(_id);
	//int size = _volume._radius * 2;
	//Texture* texture = thisObject->getGraphicsComp()->getTexture();
	//texture->renderScaled(renderer, _volume._position.x - _volume._radius, _volume._position.y - _volume._radius, size, size);
	//SDL_RenderPresent(renderer);

	//}
}

void PoolBallPhysicsComponent::update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList, std::list<float>* angularImpulsesList) {

	double frametime = physicsSystem->getThisFrametime() / 10000.0f;

	// apply linear impulses and forces
	Vec2 sumOfForces = Vec2();
	for (auto itr = forcesList->begin(); itr != forcesList->end(); itr++) {
		sumOfForces += *itr;
	}
	for (auto itr = impulsesList->begin(); itr != impulsesList->end(); itr++) {
		sumOfForces += *itr;
	}
	impulsesList->clear();

	_linearVelocity += sumOfForces;
	_prevPosition = _volume._position;

	// apply angular impulses
	float sumOfAngImpulses = 0.0f;
	for (auto itr = angularImpulsesList->begin(); itr != angularImpulsesList->end(); itr++) {
		sumOfAngImpulses += *itr;
	}
	angularImpulsesList->clear();

	_angularVelocity +=  sumOfAngImpulses;

	move(frametime);

	ContactInfo* cInfo = new ContactInfo();
	GameObject* thisObject = app->getObject(this->_id);
	if (physicsSystem->checkCollisionWithObject(thisObject, cInfo)) {
		resolveCollision(cInfo);
	}
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
	// convert radians into degrees
	float degrees = _physics->_orientation._z * 360.0f / (2.0f * PI);

	_texture->renderScaledEx(_puddleRenderer->getRenderer(), _physics->_volume._position.x - _physics->_volume._radius, _physics->_volume._position.y - _physics->_volume._radius, size, size, degrees);

	// DEBUG
	//SDL_SetRenderDrawColor(_puddleRenderer->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x, 0, _physics->_volume._position.x, 1050);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y, 1900, _physics->_volume._position.y);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y + _physics->_volume._radius, 1900, _physics->_volume._position.y + _physics->_volume._radius);
}