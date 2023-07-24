#pragma once

#include "MathCore.h"
#include "Vec3.h"

struct Box3 {
    static Box3 WithRadius( const Vec3& radius );
    static Box3 FromCenterWithRadius( const Vec3& position, const Vec3& radius );
private:
	bool _empty = true;
	Vec3 _min;
	Vec3 _max;
public:
    explicit Box3();
    Box3( const Box3& src );
    explicit Box3( const Vec3& v1, const Vec3& v2 );
public:
	void reset();
    void unite( const Box3& b );
	void unite( const Vec3& vec );
	void unite( const glm::vec3& vec );
public:
	bool isEmpty() const{ return _empty; }
	const Vec3& min() const{ assert( !isEmpty() ); return _min; }
	const Vec3& max() const{ assert( !isEmpty() ); return _max; }
	Vec3 center() const;
	Vec3 size() const;
	Vec3 radius() const;
public:
    void operator=( const Box3& src ){
		_empty = src._empty;
		_min = src._min;
		_max = src._max;
	}
	void set( const Vec3& a, const Vec3& b ){
		reset();
		unite( a );
		unite( b );
	}
public:
    bool isInside( const Vec3& v ) const{ return isInside( v.asGlm() ); }
    bool isInside( const glm::vec3& v ) const{
		if( v.x < _min.x() )return false;
		if( v.y < _min.y() )return false;
		if( v.z < _min.z() )return false;
		if( v.x > _max.x() )return false;
		if( v.y > _max.y() )return false;
		if( v.z > _max.z() )return false;
		return true;
	}
public:
	void adjust( float b ){
		assert( 0 < b );
		unite( min() - Vec3( b, b, b ) );
		unite( max() + Vec3( b, b, b ) );
	}
public:
	void operator*=( float scale ){
		_min *= scale;
		_max *= scale;
	}
public:	
	std::string toString() const;
};

std::ostream &operator<<( std::ostream &os, const Box3& v );
