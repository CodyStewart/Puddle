#pragma once

#include <list>

#include <SDL.h>

#include "commonTypedefs.h"
#include "texture.h"

struct GameObject;

enum ShapeType {
	UNKNOWN = 0,
	CIRCLE,
	RECT
};

struct ColliderData {	// we use this struct to get relevant data from the shape
	int radius = 0;
	int width = 0;
	int height = 0;
};

struct Point {
	float x;
	float y;
};

struct Vec2 {
	float _x;
	float _y;
	float _length;

	Vec2();
	Vec2(float x, float y);

	float dot(Vec2 other);
	
	void calculateMagnitude() { _length = sqrt(_x * _x + _y * _y); }
	Vec2 normalize();

	Vec2 operator-();
	Vec2 operator-(Vec2 other);
	Vec2 operator+(Vec2 other);
	void operator+=(Vec2 other);
	Vec2 operator=(Vec2 assign);
	Vec2 operator*(float scaler);
	Vec2 operator*=(float scaler);
	bool operator==(Vec2 vec);
	bool operator!=(Vec2 vec);
};

struct ContactInfo {
	Vec2 ApointWrldSpace;
	Vec2 BpointWrldSpace;
	Vec2 ApointLocalSpace;
	Vec2 BpointLocalSpace;

	Vec2 normal;
	float separationDistance;
	float timeOfImpact;

	GameObject A;
	GameObject B;
};

struct Shape {
	Point _position;

	Shape() { _position = { 0,0 }; }
};

struct Rect : public Shape {
	int _w;
	int _h;

	Rect();
	Rect(Point point, int width, int height);
};

struct Circle : public Shape {
	int _radius;
	
	Circle();
	Circle(Point point, int radius);
};

struct Entity {
	uint32 _id;
	
	Entity();
	~Entity();
};

struct EntityGenerator {
	Entity generatateNewEntity();

private:
	uint32 _currentIDToGenerate = 0;
};

//struct CollisionCircle {
//	Circle _circle;
//	
//	CollisionCircle();
//	CollisionCircle(SDL_Point pos, float radius);
//};
//
//struct CollisionBox {
//	Rect _box;
//
//	CollisionBox();
//	CollisionBox(int x, int y);
//	CollisionBox(SDL_Point point, int width, int height);
//};

struct Component {

};

struct InputComponent {
	~InputComponent() {};
	virtual void update() = 0;
};

struct GraphicsComponent {
	~GraphicsComponent() {};
	virtual Texture* getTexture() = 0;
	virtual void update() = 0;
};

struct PhysicsComponent {
	~PhysicsComponent() {};

	virtual void setID(uint32 id) = 0;
	virtual void setPosition(Point pos) = 0;
	virtual void setVelocity(Vec2 vec) = 0;
	virtual void resolveCollision(GameObject* objectCollided) = 0;
	virtual Vec2 getVelocity() = 0;
	virtual Vec2 getNormal() = 0;
	virtual Shape getCollider() = 0;
	virtual ColliderData getData() = 0;
	virtual Point getPosition() = 0;
	virtual void update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList) = 0;
};