#include "gameLogic.h"
#include "macros.h"

Vec2::Vec2() {
	_x = 0;
	_y = 0;
	_length = 0;
}

Vec2::Vec2(float x, float y) {
	_x = x;
	_y = y;
	_length = sqrt(_x * _x + _y * _y);
}

void Vec2::operator+=(Vec2 other) {
	this->_x += other._x;
	this->_y += other._y;
	this->_length = sqrt(this->_x * this->_x + this->_y * this->_y);
}

Rect::Rect() {
	_w = 0;
	_h = 0;
	_point = { 0,0 };
}

Rect::Rect(SDL_Point point, int width, int height) {
	_w = width;
	_h = height;
	_point = point;
}

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
	_box._point.x = 0;
	_box._point.y = 0;
	_box._w = 0;
	_box._h = 0;
}

CollisionBox::CollisionBox(int x, int y) {
	_box._point.x = x;
	_box._point.y = y;
	_box._w = 0;
	_box._h = 0;
}

CollisionBox::CollisionBox(SDL_Point point, int width, int height) {
	_box._point.x = point.x;
	_box._point.y = point.y;
	_box._w = width;
	_box._h = height;
}