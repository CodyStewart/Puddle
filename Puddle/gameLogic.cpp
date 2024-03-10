#include "gameLogic.h"
#include "macros.h"

Circle::Circle() {
	_point = SDL_Point();
	_radius = 0.0f;
}

Circle::Circle(SDL_Point point, float radius) {
	_point = point;
	_radius = radius;
}

Entity::Entity() {
	_id = UINTMAX;	// we use the maximum value in order to tell if something went wrong during generation of an entity
}

Entity::~Entity() {

}

Entity EntityGenerator::generatateNewEntity() {
	Entity newEntity;
	newEntity._id = _currentIDToGenerate;
	++_currentIDToGenerate;

	return newEntity;
}

CollisionCircle::CollisionCircle() {
	_circle = Circle();
}

CollisionCircle::CollisionCircle(SDL_Point pos, float radius) {
	_circle = Circle(pos, radius);
}

CollisionBox::CollisionBox() {
	_box.x = 0;
	_box.y = 0;
	_box.w = 0;
	_box.h = 0;
}

CollisionBox::CollisionBox(int x, int y) {
	_box.x = x;
	_box.y = y;
}
