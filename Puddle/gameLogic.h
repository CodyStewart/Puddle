#pragma once

#include <SDL.h>

#include "commonTypedefs.h"

struct Vec2 {
	float _x;
	float _y;
	float _length;

	Vec2();
	Vec2(float x, float y);

	void operator+=(Vec2 other);
};

struct Rect {
	SDL_Point _point;
	int _w;
	int _h;

	Rect();
	Rect(SDL_Point point, int width, int height);
};

struct Circle {
	SDL_Point _point;
	float _radius;
	
	Circle();
	Circle(SDL_Point point, float radius);
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

struct CollisionCircle {
	Circle _circle;
	
	CollisionCircle();
	CollisionCircle(SDL_Point pos, float radius);
};

struct CollisionBox {
	Rect _box;

	CollisionBox();
	CollisionBox(int x, int y);
	CollisionBox(SDL_Point point, int width, int height);
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