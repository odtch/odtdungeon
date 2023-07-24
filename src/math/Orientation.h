#pragma once

#include "MathCore.h"
#include "Vec3.h"
#include "Mat4.h"

struct Orientation {
private:
	Vec3 _direction;
	Vec3 _up;
	mutable Vec3 _right;
private:
	mutable bool _matrix_valid;
	mutable Mat4 _matrix;
public:
	explicit Orientation();
	explicit Orientation( const Vec3& direction, const Vec3& up );
	Orientation( const Orientation& src );
	explicit Orientation( const Mat4& src );
public:
//	const glm::quat& rotation() const { return _rotation; }
//	void setRotation( const glm::quat& rotation );
public:
	const glm::quat quat() const;
	const Mat4& matrix() const;
public:
	//void set( const Vec3& position, const glm::quat& rotation );
	void set( const Vec3& direction, const Vec3& up );
	void set( const Mat4& matrix );
public:
	void operator=( const Orientation& src );
public:
	const Vec3& direction() const { return _direction; }
	const Vec3& up() const { return _up; }
	const Vec3& right() const { matrix(); return _right; }
	const Vec3& axis( int a ) const;
public:
	Vec3 left() const { return right() * -1; }
public:
	void rotate( float angle, const Vec3& axis );
	Orientation rotated( float angle, const Vec3& axis ) const;
	void rotate( const glm::quat& rotation );
public:
	Orientation calcRightAngled( const Orientation& o ) const;
	Vec3 vector( int i ) const;
public:
	static bool Equals( const Orientation& a, const Orientation& b, float max_dist = 0.001f );
public:
	static Orientation Simple( const Vec3& direction );
	static Orientation Interpolated( const Orientation& a, const Orientation& b, float f );
public:
	std::string toString() const;
};

