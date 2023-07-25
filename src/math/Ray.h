#pragma once

#include "MathCore.h"
#include "Vec3.h"

struct Ray {
private:
	Vec3 _origin;
	Vec3 _direction;
public:
	explicit Ray();
	explicit Ray( const Vec3& origin, const Vec3& direction );
	explicit Ray( const Ray& src );
	explicit Ray( const PosOri& src );
public:
	const Vec3& origin() const{ return _origin; }
	const Vec3& direction() const{ return _direction; }
public:
	void setOrigin( const Vec3& origin );
	void setDirection( const Vec3& direction );
	void set( const Vec3& origin, const Vec3& direction );
	void set( const Ray& ray );
	void operator=( const Ray& ray );
public:
	Vec3 pointInDistance( float distance ) const;
	float closestPointDistance( const Vec3& point ) const;
public:
	std::string toString() const;
};

std::ostream &operator<<( std::ostream &os, const Ray& ray );
