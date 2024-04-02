#include "gameLogic.h"
#include "macros.h"

Vec2 Point::toVec() {
	return Vec2(x, y);
}

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

float Vec2::perpDot(Vec2 other) {
	return this->_x * other._y - this->_y * other._x;
}

Vec2 Vec2::getPerp() {
	return Vec2(-(this->_y), this->_x);
}

Vec2 Vec2::normalize() {
	if (this->_length != 0.0f) {
		this->_x /= _length;
		this->_y /= _length;
		this->calculateMagnitude();
	}
	return *this;
}

Vec2& Vec2::operator=(const Vec2& rhs) {
	_x = rhs._x;
	_y = rhs._y;
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

inline float Vec2::operator[](const int idx) const {
	SDL_assert(idx >= 0 && idx < 2);
	return (&_x)[idx];
}

inline float& Vec2::operator[](const int idx) {
	SDL_assert(idx >= 0 && idx < 2);
	return (&_x)[idx];
}

Vec3::Vec3() : _x(0), _y(0), _z(0) {}

inline Vec3::Vec3(float value) : _x(value), _y(value), _z(value) {}

inline Vec3::Vec3(const Vec3& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z) {}

inline Vec3::Vec3(float X, float Y, float Z) : _x(X), _y(Y), _z(Z) {}

inline Vec3::Vec3(const float* xyz) : _x(xyz[0]), _y(xyz[1]), _z(xyz[2]) {}

Vec3& Vec3::operator=(const Vec3& rhs) {
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	return *this;
}

inline Vec3& Vec3::operator=(const float* rhs) {
	_x = rhs[0];
	_y = rhs[1];
	_z = rhs[2];
	return *this;
}

inline bool Vec3::operator==(const Vec3& rhs) const {
	if (_x != rhs._x)
		return false;
	if (_y != rhs._y)
		return false;
	if (_z != rhs._z)
		return false;

	return true;
}

inline bool Vec3::operator!=(const Vec3& rhs) const {
	if (*this == rhs)
		return false;

	return true;
}

inline Vec3 Vec3::operator+(const Vec3& rhs) const {
	Vec3 temp;
	temp._x = _x + rhs._x;
	temp._y = _y + rhs._y; 
	temp._z = _z + rhs._z;  
	return temp;
}

inline const Vec3& Vec3::operator+=(const Vec3& rhs) {
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;
	return *this;
}

inline const Vec3& Vec3::operator-=(const Vec3& rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;
	return *this;
}

inline Vec3 Vec3::operator-(const Vec3& rhs) const {
	Vec3 temp;
	temp._x = _x - rhs._x;
	temp._y = _y - rhs._y;
	temp._z = _z - rhs._z;
	return temp;
}

inline Vec3 Vec3::operator*(const float rhs) const {
	Vec3 temp;
	temp._x = _x * rhs;
	temp._y = _y * rhs;
	temp._z = _z * rhs;
	return temp;
}

inline Vec3 Vec3::operator/(const float rhs) const {
	Vec3 temp;
	temp._x = _x / rhs;
	temp._y = _y / rhs;
	temp._z = _z / rhs;
	return temp;
}

inline const Vec3& Vec3::operator*=(const float rhs) {
	_x *= rhs;
	_y *= rhs;
	_z *= rhs;
	return *this;
}

inline const Vec3& Vec3::operator/=(const float rhs) {
	_x /= rhs;
	_y /= rhs;
	_z /= rhs;
	return *this;
}

inline float Vec3::operator[](const int idx) const {
	SDL_assert(idx >= 0 && idx < 3);
	return (&_x)[idx];
}

inline float& Vec3::operator[](const int idx) {
	SDL_assert(idx >= 0 && idx < 3);
	return (&_x)[idx];
}

inline Vec3 Vec3::Cross(const Vec3& rhs) const {
	// cross product is A x B, where this is A and rhs is B
	Vec3 temp;
	temp._x = (_y * rhs._z) - (rhs._y * _z);
	temp._y = (rhs._x * _z) - (_x  * rhs._z);
	temp._z = (_x * rhs._y) - (rhs._x * _y);
	return temp;
}

inline float Vec3::Dot(const Vec3& rhs) const {
	float temp = (_x * rhs._x) + (_y * rhs._y) + (_z * rhs._z);
	return temp;
}

inline const Vec3& Vec3::Normalize() {
	float mag = GetMagnitude();
	SDL_assert(mag != 0.0f);
	float invMag = 1.0f / mag;

	if (0.0f * invMag == 0.0f * invMag) {
		_x *= invMag;
		_y *= invMag;
		_z *= invMag;
	}

	return *this;
}

inline float Vec3::GetMagnitude() const {
	float mag = _x * _x + _y * _y + _z * _z;
	mag = sqrt(mag);
	return mag;
}

inline bool Vec3::IsValid() const {
	if (_x * 0.0f != _x * 0.0f)
		return false;
	if (_y * 0.0f != _y * 0.0f)
		return false;
	if (_z * 0.0f != _z * 0.0f)
		return false;

	return true;
}

inline void Vec3::GetOrtho(Vec3& u, Vec3& v) const {
	Vec3 n = *this;
	n.Normalize();

	const Vec3 w = (n._z * n._z > 0.9f * 0.9f) ? Vec3(1, 0, 0) : Vec3(0, 0, 1);
	u = w.Cross(n);
	u.Normalize();

	v = n.Cross(u);
	v.Normalize();
	u = v.Cross(n);
	u.Normalize();
}

inline Mat2::Mat2(const Mat2& rhs) {
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
}

//inline Mat2::Mat2(const float* mat) {
//	rows[0] = mat + 0;
//	rows[1] = mat + 2;
//}

inline Mat2::Mat2(const Vec2& row0, const Vec2& row1) {
	rows[0] = row0;
	rows[1] = row1;
}

inline Mat2& Mat2::operator=(const Mat2& rhs) {
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	return *this;
}

inline const Mat2& Mat2::operator*=(const float rhs) {
	rows[0] *= rhs;
	rows[1] *= rhs;
	return *this;
}

inline const Mat2& Mat2::operator+=(const Mat2& rhs) {
	rows[0] += rhs.rows[0];
	rows[1] += rhs.rows[1];
	return *this;
}

inline Mat3::Mat3(const Mat3& rhs) {
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	rows[2] = rhs.rows[2];
}

inline Mat3::Mat3(const float* mat) {
	rows[0] = mat + 0;
	rows[1] = mat + 3;
	rows[2] = mat + 6;
}

inline Mat3::Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2) {
	rows[0] = row0;
	rows[1] = row1;
	rows[2] = row2;
}

inline Mat3& Mat3::operator=(const Mat3& rhs) {
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	rows[2] = rhs.rows[2];
	return *this;
}

inline const Mat3& Mat3::operator*=(const float rhs) {
	rows[0] *= rhs;
	rows[1] *= rhs;
	rows[2] *= rhs;
	return *this;
}

inline const Mat3& Mat3::operator+=(const Mat3& rhs) {
	rows[0] += rhs.rows[0];
	rows[1] += rhs.rows[1];
	rows[2] += rhs.rows[2];
	return *this;
}

inline void Mat3::Zero() {
	rows[0].Zero();
	rows[1].Zero();
	rows[2].Zero();
}

inline void Mat3::Identity() {
	rows[0] = Vec3(1, 0, 0);
	rows[1] = Vec3(0, 1, 0);
	rows[2] = Vec3(0, 0, 1);
}

inline float Mat3::Trace() const {
	const float xx = rows[0][0] * rows[0][0];
	const float yy = rows[1][1] * rows[1][1];
	const float zz = rows[2][2] * rows[2][2];
	return (xx + yy + zz);
}

inline float Mat3::Determinant() const {
	const float i = rows[0][0] * (rows[1][1] * rows[2][2] - rows[1][2] * rows[2][1]);
	const float j = rows[0][1] * (rows[1][0] * rows[2][2] - rows[1][2] * rows[2][0]);
	const float k = rows[0][2] * (rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0]);
	return (i - j + k);
}

inline Mat3 Mat3::Transpose() const {
	Mat3 transpose;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transpose.rows[i][j] = rows[j][i];
		}
	}

	return transpose;
}

inline Mat3 Mat3::Inverse() const {
	Mat3 inv;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			inv.rows[j][i] = Cofactor(i, j);	// perform the transpose with the cofactors
		}
	}

	float det = Determinant();
	float invDet = 1.0f / det;
	inv *= invDet;
	return inv;
}

inline Mat2 Mat3::Minor(const int i, const int j) const {
	Mat2 minor;

	int yy = 0;
	for (int y = 0; y < 3; y++) {
		if (y == j)
			continue;

		int xx = 0;
		for (int x = 0; x < 3; x++) {
			if (x == i) 
				continue;

			minor.rows[xx][yy] = rows[x][y];
			xx++;
		}

		yy++;
	}

	return minor;
}

inline float Mat3::Cofactor(const int i, const int j) const {
	const Mat2 minor = Minor(i, j);
	const float C = float(pow(-1, i + 1 + j + 1)) * minor.Determinant();
	return C;
}

inline Vec3 Mat3::operator*(const Vec3& rhs) const {
	Vec3 temp;
	temp[0] = rows[0].Dot(rhs);
	temp[1] = rows[1].Dot(rhs);
	temp[2] = rows[2].Dot(rhs);
	return temp;
}

inline Mat3 Mat3::operator*(const float rhs) const {
	Mat3 temp;
	temp.rows[0] = rows[0] * rhs;
	temp.rows[1] = rows[1] * rhs;
	temp.rows[2] = rows[2] * rhs;
	return temp;
}

inline Mat3 Mat3::operator*(const Mat3& rhs) const {
	Mat3 temp;
	for (int i = 0; i < 3; i++) {
		temp.rows[i]._x = rows[i]._x * rhs.rows[0]._x + rows[i]._y * rhs.rows[1]._x + rows[i]._z * rhs.rows[2]._x;
		temp.rows[i]._y = rows[i]._x * rhs.rows[0]._y + rows[i]._y * rhs.rows[1]._y + rows[i]._z * rhs.rows[2]._y;
		temp.rows[i]._z = rows[i]._x * rhs.rows[0]._z + rows[i]._y * rhs.rows[1]._z + rows[i]._z * rhs.rows[2]._z;
 	}

	return temp;
}

inline Mat3 Mat3::operator+(const Mat3& rhs) const {
	Mat3 temp;
	for (int i = 0; i < 3; i++) {
		temp.rows[i] = rows[i] + rhs.rows[i];
	}
	return temp;
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