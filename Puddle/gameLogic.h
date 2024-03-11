#pragma once

#include <SDL.h>

#include "commonTypedefs.h"

struct Vec2 {
	float _x;
	float _y;
	float _length;

	Vec2();
	Vec2(float x, float y);
};

struct Circle {
	Circle();
	Circle(SDL_Point point, float radius);
	SDL_Point _point;
	float _radius;
};

struct Entity {
	Entity();
	~Entity();
	uint32 _id;
};

struct EntityGenerator {
	Entity generatateNewEntity();

private:
	uint32 _currentIDToGenerate = 0;
};

struct CollisionCircle {
	CollisionCircle();
	CollisionCircle(SDL_Point pos, float radius);

	Circle _circle;
};

struct CollisionBox {
	CollisionBox();
	CollisionBox(int x, int y);

	SDL_Rect _box;
};

struct Component {

};

struct InputComponent {
	~InputComponent() {};
	virtual void update() = 0;
};

struct GraphicsComponent {
	~GraphicsComponent() {};
	virtual void update() = 0;
};

struct PhysicsComponent {
	~PhysicsComponent() {};
	virtual void update() = 0;
};