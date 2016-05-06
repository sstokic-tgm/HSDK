#include "Util_Vector.hpp"

Vector2D::Vector2D(Vector2D &&v)
{
	x = v.x;
	y = v.y;

	v.x = 0.0f;
	v.y = 0.0f;
}

float *Vector2D::base()
{
	return (float*)this;
}

float const *Vector2D::base() const
{
	return (float const*)this;
}

float Vector2D::operator[](int i) const
{
	return ((float*)this)[i];
}

float &Vector2D::operator[](int i)
{
	return ((float*)this)[i];
}

void Vector2D::clear()
{
	this->x = this->y = 0.0f;
}

float Vector2D::lengthSqr()
{
	return (pow(this->x, 2) + pow(this->y, 2));
}

Vector2D Vector2D::operator+(const Vector2D &v) const
{
	return Vector2D(this->x + v.x, this->y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D &v) const
{
	return Vector2D(this->x - v.x, this->y - v.y);
}

Vector2D Vector2D::operator*(const Vector2D &v) const
{
	return Vector2D(this->x * v.x, this->y * v.y);
}

Vector2D Vector2D::operator*(float f) const
{
	return Vector2D(this->x * f, this->y * f);
}

Vector2D Vector2D::operator/(const Vector2D &v) const
{
	return Vector2D(this->x / v.x, this->y / v.y);
}

Vector2D Vector2D::operator/(float f) const
{
	return Vector2D(this->x / f, this->y / f);
}

Vector2D &Vector2D::operator+=(const Vector2D &v)
{
	this->x += v.x;
	this->y += v.y;

	return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &v)
{
	this->x -= v.x;
	this->y -= v.y;

	return *this;
}

Vector2D &Vector2D::operator*=(const Vector2D &v)
{
	this->x *= v.x;
	this->y *= v.y;

	return *this;
}

Vector2D &Vector2D::operator/=(const Vector2D &v)
{
	this->x /= v.x;
	this->y /= v.y;

	return *this;
}

Vector2D &Vector2D::operator*=(float f)
{
	this->x *= f;
	this->y *= f;

	return *this;
}

Vector2D &Vector2D::operator/=(float f)
{
	this->x /= f;
	this->y /= f;

	return *this;
}

Vector2D &Vector2D::operator=(Vector2D &&v)
{
	if (this != &v)
	{
		x = 0.0f;
		y = 0.0f;

		x = v.x;
		y = v.y;

		v.x = 0.0f;
		v.y = 0.0f;
	}

	return *this;
}

Vector2D &Vector2D::operator=(const Vector2D &v)
{
	if (this != &v)
	{
		x = v.x;
		y = v.y;
	}

	return *this;
}

Vector::Vector(Vector &&v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	v.x = 0.0f;
	v.y = 0.0f;
	v.z = 0.0f;
}

float *Vector::base()
{
	return (float*)this;
}

float const *Vector::base() const
{
	return (float const*)this;
}

float Vector::operator[](int i) const
{
	return ((float*)this)[i];
}

float &Vector::operator[](int i)
{
	return ((float*)this)[i];
}

void Vector::clear()
{
	this->x = this->y = this->z = 0.0f;
}

float Vector::lengthSqr()
{
	return (pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

Vector Vector::operator+(const Vector &v) const
{
	return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector Vector::operator-(const Vector &v) const
{
	return Vector(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector Vector::operator*(const Vector &v) const
{
	return Vector(this->x * v.x, this->y * v.y, this->z * v.z);
}

Vector Vector::operator*(float f) const
{
	return Vector(this->x * f, this->y * f, this->z * f);
}

Vector Vector::operator/(const Vector &v) const
{
	return Vector(this->x / v.x, this->y / v.y, this->z / v.z);
}

Vector Vector::operator/(float f) const
{
	return Vector(this->x / f, this->y / f, this->z / f);
}

Vector &Vector::operator+=(const Vector &v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector &Vector::operator-=(const Vector &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}

Vector &Vector::operator*=(const Vector &v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;

	return *this;
}

Vector &Vector::operator/=(const Vector &v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;

	return *this;
}

Vector &Vector::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;

	return *this;
}

Vector &Vector::operator/=(float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;

	return *this;
}

Vector &Vector::operator=(Vector &&v)
{
	if (this != &v)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		x = v.x;
		y = v.y;
		z = v.z;

		v.x = 0.0f;
		v.y = 0.0f;
		v.z = 0.0f;
	}

	return *this;
}

Vector &Vector::operator=(const Vector &v)
{
	if (this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	return *this;
}

VectorAligned::VectorAligned(vec_t x, vec_t y, vec_t z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

VectorAligned::VectorAligned(const Vector &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

VectorAligned &VectorAligned::operator=(const Vector &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;

	return *this;
}

VectorAligned &VectorAligned::operator=(const VectorAligned &v)
{
	_mm_store_ps(this->base(), _mm_load_ps(v.base()));

	return *this;
}