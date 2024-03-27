#pragma once

#include "gameLogic.h"
#include "gameObjects.h"
#include "puddleRenderer.h"

struct PuddleRenderer;

struct PoolBallInputComponent : InputComponent {
	~PoolBallInputComponent() {};
	virtual void update();
};

struct PoolBallPhysicsComponent : PhysicsComponent {
	friend struct PoolBallGraphicsComponent;

	PoolBallPhysicsComponent(Point pos, float radius, float restitution);
	~PoolBallPhysicsComponent();

	void setID(uint32 id) { _id = id; }
	void setPosition(Point pos);
	void setOrientation(Vec3 orient);
	void setLinearVelocity(Vec2 newVel);
	void setAngularVelocity(float newVel);
	virtual void setElasticity(float restitution) { _elasticity = restitution; };
	void resolveCollision(ContactInfo* info);
	virtual void update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList, std::list<float>* angularImpulsesList);

	Vec2 getLinearVelocity() { return _linearVelocity; }
	float getAngularVelocity() { return _angularVelocity; }
	Vec2 getNormal() { printf("someone is trying to get the normal from a ball\n"); return Vec2(); }	// NOTE: normal depends on collision and thus is calculated in the physics system
	Shape getCollider() { return _collisionVolume; }
	ColliderData getData() { ColliderData data; data.radius = _collisionVolume._radius; return data; }
	Point getPosition() { return _volume._position; }
	float getElasticity() { return _elasticity; }

private:
	uint32 _id;

	Vec2 _linearVelocity;
	float _angularVelocity;
	float _elasticity;
	Vec3 _orientation;

	Circle _volume;
	Circle _collisionVolume;

	Point _prevPosition = { 0,0 };
	ShapeType shapeType = CIRCLE;
	bool isMovable = true;

	void move(double frametime);
};

struct PoolBallGraphicsComponent : GraphicsComponent {

	PoolBallGraphicsComponent(PuddleRenderer* renderer, PoolBallPhysicsComponent* physics, Texture* texture);
	~PoolBallGraphicsComponent();

	virtual Texture* getTexture() { return _texture; }
	virtual void update();

private:
	PuddleRenderer* _puddleRenderer;
	PoolBallPhysicsComponent* _physics;
	Texture* _texture;
};