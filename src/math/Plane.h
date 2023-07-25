#pragma once

#include "MathCore.h"
#include "Vec3.h"

struct Plane {
private:
	Vec3 _point;
	Vec3 _normal;
public:
	explicit Plane();
	explicit Plane( const Vec3& point, const Vec3& normal );
	explicit Plane( const Plane& src );
public:
	const Vec3& point() const{ return _point; }
	const Vec3& normal() const{ return _normal; }
public:
	void setPoint( const Vec3& origin );
	void setNormal( const Vec3& normal );
	void set( const Vec3& origin, const Vec3& normal );
public:
	float distanceFrom( const Vec3& point ) const;
public:
	bool intersectionDistance( const Ray& ray, float& distance ) const;
	bool intersectionPoint( const Ray& ray, Vec3& point ) const;
public:
	std::string toString() const;
};

