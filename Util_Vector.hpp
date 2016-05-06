#pragma once

#include <memory>
#include <math.h>

using vec_t = float;

//=========================================================
// 2DVector - used for many pathfinding and many other 
// operations that are treated as planar rather than 3d.
//=========================================================
class Vector2D
{
public:

	vec_t x, y;

	Vector2D() {}
	Vector2D(vec_t x, vec_t y) : x{ x }, y{ y } {}
	Vector2D(float *xy) : x{ xy[0] }, y{ xy[1] } {}
	Vector2D(const Vector2D &v) : x{ v.x }, y{ v.y } {}
	Vector2D(Vector2D &&v);

	~Vector2D() = default;

	float *base();
	float const *base() const;

	float operator[](int i) const;
	float &operator[](int i);

	void clear();

	float lengthSqr();

	Vector2D operator+(const Vector2D &v) const;
	Vector2D operator-(const Vector2D &v) const;
	Vector2D operator*(const Vector2D &v) const;
	Vector2D operator*(float f) const;
	Vector2D operator/(const Vector2D &v) const;
	Vector2D operator/(float f) const;
	Vector2D &operator+=(const Vector2D &v);
	Vector2D &operator-=(const Vector2D &v);
	Vector2D &operator*=(const Vector2D &v);
	Vector2D &operator/=(const Vector2D &v);
	Vector2D &operator*=(float f);
	Vector2D &operator/=(float f);
	Vector2D &operator=(Vector2D &&v);
	Vector2D &operator=(const Vector2D &v);
};

inline float DotProduct2D(const Vector2D &a, const Vector2D &b) { return (a.x*b.x + a.y*b.y); }

//=========================================================
// 3D Vector
//=========================================================
class Vector
{
public:

	vec_t x, y, z;

	Vector() {}
	Vector(vec_t x, vec_t y, vec_t z) : x{ x }, y{ y }, z{ z } {}
	Vector(float *xyz) : x{ xyz[0] }, y{ xyz[1] }, z{ xyz[2] } {}
	Vector(const Vector &v) : x{ v.x }, y{ v.y }, z{ v.z } {}
	Vector(Vector &&v);

	~Vector() = default;

	float *base();
	float const *base() const;

	float operator[](int i) const;
	float &operator[](int i);

	void clear();

	float lengthSqr();

	Vector operator+(const Vector &v) const;
	Vector operator-(const Vector &v) const;
	Vector operator*(const Vector &v) const;
	Vector operator*(float f) const;
	Vector operator/(const Vector &v) const;
	Vector operator/(float f) const;
	Vector &operator+=(const Vector &v);
	Vector &operator-=(const Vector &v);
	Vector &operator*=(const Vector &v);
	Vector &operator/=(const Vector &v);
	Vector &operator*=(float f);
	Vector &operator/=(float f);
	Vector &operator=(Vector &&v);
	Vector &operator=(const Vector &v);
};

inline float DotProduct(const Vector &a, const Vector &b) { return (a.x*b.x + a.y*b.y + a.z*b.z); }
inline Vector CrossProduct(const Vector &a, const Vector &b) { return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
inline void VectorTransform(float *in1, float in2[3][4], float *out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

//=========================================================
// 3D Vector Aligned
//=========================================================
class __declspec(align(16)) VectorAligned : public Vector
{
public:

	vec_t w;

	VectorAligned() {}
	VectorAligned(vec_t x, vec_t y, vec_t z);
	explicit VectorAligned(const Vector &v);

	VectorAligned &operator=(const Vector &v);
	VectorAligned &operator=(const VectorAligned &v);
};

#define vec3_t Vector