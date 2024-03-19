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

float Vec2::dot(Vec2 other) {
	return this->_x * other._x + this->_y * other._y;
}

Vec2 Vec2::normalize() {
	this->_x /= _length;
	this->_y /= _length;
	this->calculateMagnitude();
	return *this;
}

Vec2 Vec2::operator-() {
	this->_x = -1 * this->_x;
	this->_y = -1 * this->_y;
	this->calculateMagnitude();

	return *this;
}

Vec2 Vec2::operator-(Vec2 other) {
	Vec2 diffVec = Vec2();
	diffVec._x = this->_x - other._x;
	diffVec._y = this->_y - other._y;
	diffVec.calculateMagnitude();

	return diffVec;
}

Vec2 Vec2::operator+(Vec2 other) {
	Vec2 sumVec = Vec2();
	sumVec._x = this->_x + other._x;
	sumVec._y = this->_y + other._y;
	sumVec.calculateMagnitude();

	return sumVec;
}

void Vec2::operator+=(Vec2 other) {
	this->_x += other._x;
	this->_y += other._y;
	this->calculateMagnitude();
}

Vec2 Vec2::operator=(Vec2 assign) {
	this->_x = assign._x;
	this->_y = assign._y;
	this->calculateMagnitude();

	return Vec2(this->_x, this->_y);
}

Vec2 Vec2::operator*(float scaler) {
	Vec2 returnVec = Vec2();
	returnVec._x = this->_x * scaler;
	returnVec._y = this->_y * scaler;
	returnVec.calculateMagnitude();

	return returnVec;
}

Vec2 Vec2::operator*=(float scaler) {
	Vec2 returnVec = Vec2();
	returnVec._x = this->_x *= scaler;
	returnVec._y = this->_y *= scaler;

	return returnVec;
}

bool Vec2::operator==(Vec2 vec) {
	return this->_x == vec._x && this->_y == vec._y;
}

bool Vec2::operator!=(Vec2 vec) {
	return !(*this == vec);
}

Rect::Rect() {
	_position = Point();
	_w = 0;
	_h = 0;
}

Rect::Rect(Point point, int width, int height) {
	_position = point;
	_w = width;
	_h = height;
}

Circle::Circle() {
	_position = Point();
	_radius = 0;
}

Circle::Circle(Point point, int radius) {
	_position = point;
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

//CollisionCircle::CollisionCircle() {
//	_circle = Circle();
//}
//
//CollisionCircle::CollisionCircle(SDL_Point pos, float radius) {
//	_circle = Circle(pos, radius);
//}
//
//CollisionBox::CollisionBox() {
//	_box._point.x = 0;
//	_box._point.y = 0;
//	_box._w = 0;
//	_box._h = 0;
//}
//
//CollisionBox::CollisionBox(int x, int y) {
//	_box._point.x = x;
//	_box._point.y = y;
//	_box._w = 0;
//	_box._h = 0;
//}
//
//CollisionBox::CollisionBox(SDL_Point point, int width, int height) {
//	_box._point.x = point.x;
//	_box._point.y = point.y;
//	_box._w = width;
//	_box._h = height;
//}