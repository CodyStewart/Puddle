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

struct Vec2;

struct Point {
	float x;
	float y;

	Point() : x(0.0f), y(0.0f) {}
	Point(float xPos, float yPos) : x(xPos), y(yPos) {}

	Vec2 toVec();

	Point* operator=(Point rhs) { x = rhs.x; y = rhs.y; return this; }
	Point* operator+=(Point rhs) { x += rhs.x; y += rhs.y; return this; }
	Point* operator-=(Point rhs) { x -= rhs.x; y -= rhs.y; return this; }
};

struct Vec2 {
	float _x;
	float _y;
	float _length;

	Vec2();
	Vec2(float x, float y);

	Point toPoint() { return Point(_x, _y); }

	float dot(Vec2 other);
	float perpDot(Vec2 other);
	Vec2 getPerp();
	
	void calculateMagnitude() { _length = sqrt(_x * _x + _y * _y); }
	Vec2 normalize();

	Vec2& operator=(const Vec2& rhs);
	//Vec2& operator=(Vec2* rhs);
	Vec2 operator-();
	Vec2 operator-(Vec2 other);
	Vec2 operator+(Vec2 other);
	void operator+=(Vec2 other);
	Vec2 operator*(float scaler);
	Vec2 operator*=(float scaler);
	bool operator==(Vec2 vec);
	bool operator!=(Vec2 vec);
	float operator[](const int idx) const;
	float& operator[](const int idx);
};

struct Vec3 {
	float _x;
	float _y;
	float _z;

	Vec3();
	Vec3(float value);
	Vec3(const Vec3& rhs);
	Vec3(float X, float Y, float Z);
	Vec3(const float* xyz);
	Vec3& operator=(const Vec3& rhs);
	Vec3& operator=(const float* rhs);

	bool operator==(const Vec3& rhs) const;
	bool operator!=(const Vec3& rhs) const;
	Vec3 operator+(const Vec3& rhs) const;
	const Vec3& operator+=(const Vec3& rhs);
	const Vec3& operator-=(const Vec3& rhs);
	Vec3 operator-(const Vec3& rhs) const;
	Vec3 operator*(const float rhs) const;
	Vec3 operator/(const float rhs) const;
	const Vec3& operator*=(const float rhs);
	const Vec3& operator/=(const float rhs);
	float operator[](const int idx) const;
	float& operator[](const int idx);

	void Zero() { _x = 0.0f; _y = 0.0f, _z = 0.0f; }

	Vec3 Cross(const Vec3& rhs) const;
	float Dot(const Vec3& rhs) const;

	const Vec3& Normalize();

	float GetMagnitude() const;
	float GetLengthSqr() const { return Dot(*this); }
	bool IsValid() const;
	void GetOrtho(Vec3& u, Vec3& v) const;

	const float* ToPtr() const { return &_x; }
};

struct Mat2 {
	Vec2 rows[2];

	Mat2() {}
	Mat2(const Mat2& rhs);
	//Mat2(const float* mat);
	Mat2(const Vec2& row0, const Vec2& row1);
	
	Mat2& operator=(const Mat2& rhs);
	const Mat2& operator*=(const float rhs);
	const Mat2& operator+=(const Mat2& rhs);

	float Determinant() const { return rows[0]._x * rows[1]._y - rows[0]._y * rows[1]._x; }
};

struct Mat3 {
	Vec3 rows[3];

	Mat3() {}
	Mat3(const Mat3& rhs);
	Mat3(const float* mat);
	Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2);
	Mat3& operator=(const Mat3& rhs);

	void Zero();
	void Identity();

	float Trace() const;
	float Determinant() const;
	Mat3 Transpose() const;
	Mat3 Inverse() const;
	Mat2 Minor(const int i, const int j) const;
	float Cofactor(const int i, const int j) const;

	Vec3 operator*(const Vec3& rhs) const;
	Mat3 operator*(const float rhs) const;
	Mat3 operator*(const Mat3& rhs) const;
	Mat3 operator+(const Mat3& rhs) const;
	const Mat3& operator*=(const float rhs);
	const Mat3& operator+=(const Mat3& rhs);
};

struct Quat {
	float w;
	float x;
	float y;
	float z;

	Quat();
	Quat(const Quat& rhs);
	Quat(float X, float Y, float Z, float W);
	Quat(Vec3 n, const float angleRadians);
	const Quat& operator = (const Quat& rhs);

	Quat& operator *= (const float& rhs);
	Quat& operator *= (const Quat& rhs);
	Quat	operator * (const Quat& rhs) const;

	void	Normalize();
	void	Invert();
	Quat	Inverse() const;
	float	MagnitudeSquared() const;
	float	GetMagnitude() const;
	Vec3	RotatePoint(const Vec3& rhs) const;
	Mat3	RotateMatrix(const Mat3& rhs) const;
	Vec3	xyz() const { return Vec3(x, y, z); }
	bool	IsValid() const;

	Mat3	ToMat3() const;
	//Vec4	ToVec4() const { return Vec4(w, x, y, z); }
};

inline Quat::Quat() :
	x(0),
	y(0),
	z(0),
	w(1) {
}

inline Quat::Quat(const Quat& rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z),
	w(rhs.w) {
}

inline Quat::Quat(float X, float Y, float Z, float W) :
	x(X),
	y(Y),
	z(Z),
	w(W) {
}

inline Quat::Quat(Vec3 n, const float angleRadians) {
	const float halfAngleRadians = 0.5f * angleRadians;

	w = cosf(halfAngleRadians);

	const float halfSine = sinf(halfAngleRadians);
	n.Normalize();
	x = n._x * halfSine;
	y = n._y * halfSine;
	z = n._z * halfSine;
}

inline const Quat& Quat::operator = (const Quat& rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

inline Quat& Quat::operator *= (const float& rhs) {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

inline Quat& Quat::operator *= (const Quat& rhs) {
	Quat temp = *this * rhs;
	w = temp.w;
	x = temp.x;
	y = temp.y;
	z = temp.z;
	return *this;
}

inline Quat Quat::operator * (const Quat& rhs) const {
	Quat temp;
	temp.w = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
	temp.x = (x * rhs.w) + (w * rhs.x) + (y * rhs.z) - (z * rhs.y);
	temp.y = (y * rhs.w) + (w * rhs.y) + (z * rhs.x) - (x * rhs.z);
	temp.z = (z * rhs.w) + (w * rhs.z) + (x * rhs.y) - (y * rhs.x);
	return temp;
}

inline void Quat::Normalize() {
	float invMag = 1.0f / GetMagnitude();

	if (0.0f * invMag == 0.0f * invMag) {
		x = x * invMag;
		y = y * invMag;
		z = z * invMag;
		w = w * invMag;
	}
}

inline void Quat::Invert() {
	*this *= 1.0f / MagnitudeSquared();
	x = -x;
	y = -y;
	z = -z;
}

inline Quat Quat::Inverse() const {
	Quat val(*this);
	val.Invert();
	return val;
}

inline float Quat::MagnitudeSquared() const {
	return ((x * x) + (y * y) + (z * z) + (w * w));
}

inline float Quat::GetMagnitude() const {
	return sqrtf(MagnitudeSquared());
}

inline Vec3 Quat::RotatePoint(const Vec3& rhs) const {
	Quat vector(rhs._x, rhs._y, rhs._z, 0.0f);
	Quat final = *this * vector * Inverse();
	return Vec3(final.x, final.y, final.z);
}

inline bool Quat::IsValid() const {
	if (x * 0 != x * 0) {
		return false;
	}
	if (y * 0 != y * 0) {
		return false;
	}
	if (z * 0 != z * 0) {
		return false;
	}
	if (w * 0 != w * 0) {
		return false;
	}
	return true;
}

inline Mat3 Quat::RotateMatrix(const Mat3& rhs) const {
	Mat3 mat;
	mat.rows[0] = RotatePoint(rhs.rows[0]);
	mat.rows[1] = RotatePoint(rhs.rows[1]);
	mat.rows[2] = RotatePoint(rhs.rows[2]);
	return mat;
}

inline Mat3 Quat::ToMat3() const {
	Mat3 mat;
	mat.Identity();

	mat.rows[0] = RotatePoint(mat.rows[0]);
	mat.rows[1] = RotatePoint(mat.rows[1]);
	mat.rows[2] = RotatePoint(mat.rows[2]);
	return mat;
}

struct ContactInfo {
	Vec2 ApointWrldSpace;
	Vec2 BpointWrldSpace;
	Vec2 ApointLocalSpace;
	Vec2 BpointLocalSpace;

	Vec2 normal;
	float separationDistance;
	float timeOfImpact;

	GameObject* A;
	GameObject* B;
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
	virtual void setLinearVelocity(Vec2 vec) = 0;
	virtual void setElasticity(float restitution) = 0;
	
	virtual void update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList, std::list<float>* angularImpulsesList) = 0;

	virtual Vec2 getLinearVelocity() = 0;
	virtual Vec2 getNormal() = 0;
	virtual Shape getCollider() = 0;
	virtual ColliderData getData() = 0;
	virtual Point getPosition() = 0;
	virtual float getElasticity() = 0;
};