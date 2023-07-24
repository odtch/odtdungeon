#pragma once

#include "MathCore.h"

struct Vec2 {
public:
	static const Vec2 Null;
public:
	union {
		struct { float x; float y; } data;
		glm::vec2 _vec2;
	};
public:
	explicit Vec2()
		:_vec2( 0, 0 )
	{}
	explicit Vec2( float x, float y )
		:_vec2( x, y )
	{
		ASSERT( isValid() );
	}
	Vec2( const Vec2& vec )
		:_vec2( vec._vec2 )
	{
		ASSERT( isValid() );
	}
	explicit Vec2( const glm::vec2& vec )
		:_vec2( vec )
	{
		ASSERT( isValid() );
	}
public:
	float x() const{ return _vec2.x; }
	float y() const{ return _vec2.y; }
	float& x(){ return _vec2.x; }
	float& y(){ return _vec2.y; }
public:
	void setX( float x ){ _vec2.x = x; ASSERT( isValid() ); }
	void setY( float y ){ _vec2.y = y; ASSERT( isValid() ); }
public:
	void set( float x, float y ){
		_vec2.x = x; _vec2.y = y;
	}
	void clear(){
		set( 0, 0 );
	}
public:
	float length() const{ return glm::length( _vec2 ); }
	float length2() const{ return glm::length2( _vec2 ); }
public:
	void operator *=( float s ){ _vec2 *= s; ASSERT( isValid() ); }
	Vec2 operator *( float s ) const{ return Vec2( _vec2 * s ); }
	void operator /=( float s ){ _vec2 /= s; ASSERT( isValid() ); }
	Vec2 operator /( float s ) const{ ASSERT( s != 0 ); return Vec2( _vec2 / s ); }
	Vec2 operator -() const { return Vec2( -x(), -y() ); }
	Vec2 operator -( const Vec2& o ) const{ return Vec2( x() - o.x(), y() - o.y() );}
	void operator -=( const Vec2& o ){ _vec2 -= o.asGlm(); ASSERT( isValid() ); }
	Vec2 operator +( const Vec2& o ) const{ return Vec2( x() + o.x(), y() + o.y() );}
	void operator +=( const Vec2& o ){ _vec2 += o.asGlm(); ASSERT( isValid() ); }
	void operator =( const Vec2& o ){ _vec2 = o.asGlm(); ASSERT( isValid() ); }
    void operator =( const glm::vec2& o ){ _vec2 = o; ASSERT( isValid() ); }
public:
	void normalize(){
		//std::cout << "vec2 " << toString() << ".normalize ";
		float l2 = length2();
		while( 100000.0f < l2 ){
			std::cout << "Vec2::normalize " << toString() << " is too long\n";
			(*this) *= 0.01f;
			l2 = length2();
		}
		float l = std::sqrt( l2 );
		if( l < 0.0001f ){
			std::cout << "Vec2::normalize " << toString() << " is too small\n";
			assert( false );
		}
		//std::cout << "n " << l2 << ", " << l << " " << length() << "\n";
		//_vec2 = glm::normalize( _vec2 );
		_vec2 /= l;
		//std::cout << "l=" << length() << "\n"; std::cout.flush();
		ASSERT( std::abs( length() - 1.0f ) < 0.001f );
		//		std::cout << " is " << toString() << "\n";
		//		std::cout.flush();
		//ASSERT( isValid() );
		if( !isValid() ){
		std::cout << "Vec2::normalize is not valid\n";
		std::cout.flush();
		_vec2 = glm::vec2( 0, 1 );
	}
	}
	Vec2 normalized() const{
		Vec2 n( *this );
		n.normalize();
		return n;
	}
public:
	static float Dot( const Vec2& a, const Vec2& b ){
		return glm::dot( a.asGlm(), b.asGlm() );
	}
	static Vec2 Reflection( const Vec2& in, const Vec2& normal ){
		return Vec2( glm::reflect( in.asGlm(), normal.asGlm() ) );
	}
public:
	bool equals( const Vec2& v, float maxDiff = 0.0001f ) const {
		if( maxDiff <= glm::abs( v.x() - x() ) )return false;
		if( maxDiff <= glm::abs( v.y() - y() ) )return false;
		return true;
	}
	bool isNan() const{
		if( isnanf( _vec2.x ) )return true;
		if( isnanf( _vec2.y ) )return true;
		return false;
	}
	bool isValid() const{
		if( isNan() )return false;
		if( isinff( _vec2.x ) )return false;
		if( isinff( _vec2.y ) )return false;
		return true;
	}
public:
	static Vec2 Lerp( const Vec2& a, const Vec2& b, float f );
public:
	std::string toString() const{ return std::string( "Vec2( " ) + std::to_string( x() ) + std::string( "f, " ) + std::to_string( y() ) + std::string( "f )" ); }
public:
	const glm::vec2& asGlm() const{ return _vec2; }
	operator const glm::vec2& () const{ return _vec2; }
};

std::ostream &operator<<( std::ostream &os, const Vec2& v );
