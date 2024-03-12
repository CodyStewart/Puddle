#include "poolTable.h"

void PoolWallInputComponent::update() {

}

PoolWallPhysicsComponent::PoolWallPhysicsComponent(SDL_Point pos, int width, int height) {
	_velocity = Vec2();
	_volume = Rect(pos, width, height);
	_collisionVolume = CollisionBox(pos, width, height);
}

PoolWallPhysicsComponent::~PoolWallPhysicsComponent() {

}

//void PoolWallPhysicsComponent::move(PoolWallPhysicsComponent* physics) {
//	physics->_volume._point.x += static_cast<int>(_velocity._x);
//	physics->_volume._point.y += static_cast<int>(_velocity._y);
//	physics->_collisionVolume._circle._point = physics->_volume._point;
//}

//extern PhysicsSystem* physicsSystem;

void PoolWallPhysicsComponent::update() {
	//_volume._point.x += 1;
	//Vec2 gravity = physicsSystem->calculateGravity();
	//_velocity += gravity;
	//checkCollisions(world);
	//move(this);
}

PoolWallGraphicsComponent::PoolWallGraphicsComponent(PuddleRenderer* renderer, PoolWallPhysicsComponent* physics, Texture* texture) {
	_puddleRenderer = renderer;
	_physics = physics;
	_texture = texture;
}

PoolWallGraphicsComponent::~PoolWallGraphicsComponent() {
	if (_texture) {
		delete _texture;
		_texture = nullptr;
	}
}

void PoolWallGraphicsComponent::update() {
	_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._point.x, _physics->_volume._point.y, _physics->_volume._w, _physics->_volume._h);
	//float size = _physics->_volume._radius * UNIT_SIZE;
	//_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._point.x, _physics->_volume._point.y, static_cast<int>(size), static_cast<int>(size));
}