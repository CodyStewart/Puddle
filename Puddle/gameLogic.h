#pragma once

#include <SDL.h>

#include "commonTypedefs.h"

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

struct Component {

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