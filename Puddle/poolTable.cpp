#include "poolTable.h"

void PoolWallInputComponent::update() {

}

PoolWallPhysicsComponent::PoolWallPhysicsComponent(Point pos, int width, int height, Vec2 normal, float restitution) {
	_linearVelocity = Vec2();
	_volume = Rect(pos, width, height);
	_collisionVolume = Rect(pos, width, height);
	_normal = normal;
	_elasticity = restitution;
}

PoolWallPhysicsComponent::~PoolWallPhysicsComponent() {

}

//void PoolWallPhysicsComponent::move(PoolWallPhysicsComponent* physics) {
//	physics->_volume._point.x += static_cast<int>(_velocity._x);
//	physics->_volume._point.y += static_cast<int>(_velocity._y);
//	physics->_collisionVolume._circle._point = physics->_volume._point;
//}

//extern PhysicsSystem* physicsSystem;

void PoolWallPhysicsComponent::setPosition(Point pos) {
	_volume._position = pos;
	_collisionVolume._position = pos;
}

void PoolWallPhysicsComponent::resolveCollision(ContactInfo* cInfo) {
	return; // PoolWalls don't need to react to collisions
}

void PoolWallPhysicsComponent::update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList, std::list<float>* angularImpulsesList) {
	// walls do not move in this game, but we are still adding impulses in case of expansion
	// for now, clear the lists
	forcesList->clear();
	impulsesList->clear();
	angularImpulsesList->clear();
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

void PoolWallGraphicsComponent::update(float leftoverTime) {
	_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._position.x, _physics->_volume._position.y, _physics->_volume._w, _physics->_volume._h);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x, 0, _physics->_volume._position.x, 1050);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y, 1900, _physics->_volume._position.y);

	//float size = _physics->_volume._radius * UNIT_SIZE;
	//_texture->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._point.x, _physics->_volume._point.y, static_cast<int>(size), static_cast<int>(size));
}