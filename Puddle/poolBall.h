#pragma once
#include <SDL.h>
#include <zlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#include "gameLogic.h"
#include "gameObjects.h"
#include "puddleRenderer.h"

struct PuddleRenderer;

struct BallSampler {
	SDL_Point _centerOfBall = { 0,0 };
	float _radius = 0.0;
	Vec2 _prevOrientation = Vec2();

	char* _samples = nullptr;
	int _width = 0;	// width of the image
	int _height = 0;	// height of the image
	int _size = 0;	// size of the image. This is width * height * (depth * 8)
	int _depth = 0;	// depth of each pixel. Measured as number of 8-bit components (thus RBGA = 4)

	bool withinSampler(int x, int y);
	//bool withinOutsideSamplerRing(int x, int y);
	//bool withinMiddleSamplerRing(int x, int y);
	//bool withinInsideSamplerRing(int x, int y);
};

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

	PoolBallGraphicsComponent(PuddleRenderer* renderer, PoolBallPhysicsComponent* physics, ResHandleShrdPtr rawDataHandle);
	~PoolBallGraphicsComponent();

	virtual Texture* getTexture() { return _texture; }
	virtual void update();

private:
	PuddleRenderer* _puddleRenderer;
	PoolBallPhysicsComponent* _physics;
	Texture* _texture;
	Texture* _ring;
	ResHandleShrdPtr _handle;
	RawImageData _imgData;
	BallSampler _sampler;

	Texture* computeOrientedTexture();
};