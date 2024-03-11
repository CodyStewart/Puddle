#include "poolBall.h"

void PoolBallInputComponent::update() {
	int x = 0;
}

PoolBallPhysicsComponent::PoolBallPhysicsComponent(SDL_Point pos, float radius) {
	_volume = Circle(pos, radius);
	_collisionVolume = CollisionCircle(pos, radius);
}

PoolBallPhysicsComponent::~PoolBallPhysicsComponent() {
	
}

void PoolBallPhysicsComponent::move(PoolBallPhysicsComponent* physics, SDL_Point point) {
	physics->_volume._point = point;
	physics->_collisionVolume._circle._point = point;
}

void PoolBallPhysicsComponent::update() {
	_volume._point.x += 1;
	move(this, _volume._point);
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