#include "poolBall.h"
#include "physics.h"

void PoolBallInputComponent::update() {

}

PoolBallPhysicsComponent::PoolBallPhysicsComponent(SDL_Point pos, float radius) {
	_velocity = Vec2();
	_volume = Circle(pos, radius);
	_collisionVolume = CollisionCircle(pos, radius);
}

PoolBallPhysicsComponent::~PoolBallPhysicsComponent() {
	
}

void PoolBallPhysicsComponent::move(PoolBallPhysicsComponent* physics) {
	physics->_volume._point.x += static_cast<int>(_velocity._x);
	physics->_volume._point.y += static_cast<int>(_velocity._y);
	physics->_collisionVolume._circle._point = physics->_volume._point;
}

extern PhysicsSystem* physicsSystem;

void PoolBallPhysicsComponent::update() {
	//_volume._point.x += 1;
	Vec2 gravity = physicsSystem->calculateGravity();
	_velocity += gravity;
	//checkCollisions(world);
	move(this);
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
	float size = _physics->_volume._radius * UNIT_SIZE;
	_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._point.x, _physics->_volume._point.y, static_cast<int>(size), static_cast<int>(size));
}