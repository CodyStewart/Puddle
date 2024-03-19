#pragma once

#include "gameLogic.h"
#include "gameObjects.h"
#include "puddleRenderer.h"

/* Defines the Pool Table and Pool Wall colliders
*
*
*/

struct PoolWallInputComponent : InputComponent {
	~PoolWallInputComponent() {};
	virtual void update();
};

struct PoolWallPhysicsComponent : PhysicsComponent {
	friend struct PoolWallGraphicsComponent;

	PoolWallPhysicsComponent(Point pos, int width, int height, Vec2 normal);
	~PoolWallPhysicsComponent();

	void setID(uint32 id) { _id = id; }
	void setPosition(Point pos);
	void resolveCollision(GameObject* objectCollided);
	void setVelocity(Vec2 vec) { return; }
	virtual void update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList);

	Vec2 getVelocity() { return _velocity; }
	Vec2 getNormal() { return _normal; }
	Shape getCollider() { return _collisionVolume; }
	ColliderData getData() { ColliderData data; data.width = _collisionVolume._w; data.height = _collisionVolume._h; return data; }
	Point getPosition() { return _volume._position; }

private:
	Vec2 _normal;
	Vec2 _velocity;
	Rect _volume;
	Rect _collisionVolume;
	uint32 _id;

	//void move(PoolWallPhysicsComponent* physics);
};

struct PoolWallGraphicsComponent : GraphicsComponent {
	PoolWallGraphicsComponent(PuddleRenderer* renderer, PoolWallPhysicsComponent* physics, Texture* texture);
	~PoolWallGraphicsComponent();

	virtual Texture* getTexture() { return _texture; }
	virtual void update();

private:
	PuddleRenderer* _puddleRenderer;
	PoolWallPhysicsComponent* _physics;
	Texture* _texture;
};

//struct PoolTable {
//
//};