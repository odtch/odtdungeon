#pragma once

#include "MathCore.h"
#include "Vec2.h"

struct Vec3 {
public:
	static const Vec3 Null;
	static const Vec3 Forward;
	static const Vec3 Backward;
	static const Vec3 Right;
	static const Vec3 Left;
	static const Vec3 Up;
	static const Vec3 Down;
	static const Vec3 OneOneOne;
private:
	union {
		struct { float x; float y; float z; } data;
		glm::vec3 _vec3;
	};
public:
	explicit Vec3()
		:_vec3( 0, 0, 0 )
	{}
	explicit Vec3( float x, float y, float z )
		:_vec3( x, y, z )
	{
		ASSERT_VALID();
	}
	Vec3( const Vec3& vec )
		:_vec3( vec._vec3 )
	{
		ASSERT_VALID();
	}
	explicit Vec3( const glm::vec3& vec )
		:_vec3( vec )
	{
		ASSERT_VALID();
	}
public:
	float x() const{ return _vec3.x; }
	float y() const{ return _vec3.y; }
	float z() const{ return _vec3.z; }
	float& x(){ return _vec3.x; }
	float& y(){ return _vec3.y; }
	float& z(){ return _vec3.z; }
public:
	void setX( float x ){ _vec3.x = x; ASSERT_VALID(); }
	void setY( float y ){ _vec3.y = y; ASSERT_VALID(); }
	void setZ( float z ){ _vec3.z = z; ASSERT_VALID(); }
public:
	void set( float x, float y, float z ){
		_vec3.x = x; _vec3.y = y; _vec3.z = z;
	}
	void clear(){
		set( 0, 0, 0 );
	}
public:
	float length() const{ return glm::length( _vec3 ); }
	float length2() const{ return glm::length2( _vec3 ); }
public:
	void operator *=( float s ){ _vec3 *= s; ASSERT_VALID(); }
	Vec3 operator *( float s ) const{ return Vec3( _vec3 * s ); }
	Vec3 operator /( float s ) const{ ASSERT( s != 0 ); return Vec3( _vec3 / s ); }
	void operator /=( float s ){ _vec3 /= s; ASSERT_VALID(); }
	Vec3 operator -() const { return Vec3( -x(), -y(), -z() ); }
	Vec3 operator -( const Vec3& o ) const{ return Vec3( x() - o.x(), y() - o.y(), z() - o.z() );}
	void operator -=( const Vec3& o ){ _vec3 -= o.asGlm(); ASSERT_VALID(); }
	Vec3 operator +( const Vec3& o ) const{ return Vec3( x() + o.x(), y() + o.y(), z() + o.z() );}
	void operator +=( const Vec3& o ){ _vec3 += o.asGlm(); ASSERT_VALID(); }
	void operator =( const Vec3& o ){ _vec3 = o.asGlm(); ASSERT_VALID(); }
    void operator =( const glm::vec3& o ){ _vec3 = o; ASSERT_VALID(); }
	Vec3 operator,( const Vec3& v1 ) = delete;
public:
	void normalize(){
		//std::cout << "vec3 " << toString() << ".normalize ";
		float l = length();
		while( 100000.0f < l ){
			std::cout << "Vec3::normalize " << toString() << " is too long\n";
			(*this) *= 0.01f;
			l = length();
		}
		//float l = std::sqrt( l2 );
		if( l < 0.0001f ){
			std::cout << "Vec3::normalize " << toString() << " is too small\n";
			//l = 0.0001f;
			//assert( false );
		}
		//std::cout << "n " << l2 << ", " << l << " " << length() << "\n";
		//_vec3 = glm::normalize( _vec3 );
		_vec3 /= l;
		//std::cout << "l=" << length() << "\n"; std::cout.flush();
		ASSERT( std::abs( length() - 1.0f ) < 0.001f );
//		std::cout << " is " << toString() << "\n";
//		std::cout.flush();
		//ASSERT_VALID();
		if( !isValid() ){
			std::cout << "Vec3::normalize " << toString() << " is not valid\n";
			std::cout.flush();
			_vec3 = glm::vec3( 0, 0, 1 );
		}
	}
	Vec3 normalized() const{
		Vec3 n( *this );
		n.normalize();
		return n;
	}
public:
	float& operator[]( int i ){
		assert( 0 <= i && i < 3 );
		return _vec3[i];
	}
	float operator[]( int i ) const {
		assert( 0 <= i && i < 3 );
		return _vec3[i];
	}
public:
	Vec3 xed( float v ) const{ return Vec3(  v , y(), z() ); }
	Vec3 yed( float v ) const{ return Vec3( x(),  v , z() ); }
	Vec3 zed( float v ) const{ return Vec3( x(), y(),  v  ); }
public:
	int highestIndex() const{
		if( _vec3[0] > _vec3[1] && _vec3[0] > _vec3[2] )
			return 0;
		if( _vec3[1] > _vec3[2] )
			return 1;
		return 2;
	}
	float sum() const{ return data.x + data.y + data.z; }
public:
	void ASSERT_VALID() const {
		if( !isValid() ){
			std::cout << "Vec3 not valid " << toString() << "\n";
			std::cout.flush();
			assert( false );
		}
	}
public:
	static Vec3 Cross( const Vec3& a, const Vec3& b ){
		return Vec3( glm::cross( a.asGlm(), b.asGlm() ) );
	}
	static Vec3 Normal( const Vec3& a, const Vec3& b, const Vec3& c ){
		return Cross( ( c - a ).normalized(), ( b - a ).normalized() ).normalized();
	}
	static float Dot( const Vec3& a, const Vec3& b ){
		return glm::dot( a.asGlm(), b.asGlm() );
	}
	static Vec3 Reflection( const Vec3& in, const Vec3& normal ){
		return Vec3( glm::reflect( in.asGlm(), normal.asGlm() ) );
	}
public:
	bool equals( const Vec3& v, float maxDiff = 0.0001f ) const {
		if( maxDiff <= glm::abs( v.x() - x() ) )return false;
		if( maxDiff <= glm::abs( v.y() - y() ) )return false;
		if( maxDiff <= glm::abs( v.z() - z() ) )return false;
		return true;
	}
	bool isNan() const{
		if( isnanf( _vec3.x ) )return true;
		if( isnanf( _vec3.y ) )return true;
		if( isnanf( _vec3.z ) )return true;
		return false;
	}
	bool isValid() const{
		if( isNan() )return false;
		if( isinff( _vec3.x ) )return false;
		if( isinff( _vec3.y ) )return false;
		if( isinff( _vec3.z ) )return false;
		return true;
	}
public:
	static Vec3 Lerp( const Vec3& a, const Vec3& b, float f );
public:
	glm::vec4 toVec40() const{ return glm::vec4( data.x, data.y, data.z, 0 ); }
public:
	std::string toString() const{ return std::string( "Vec3( " ) + std::to_string( x() ) + std::string( "f, " ) + std::to_string( y() ) + std::string( "f, " ) + std::to_string( z() ) + std::string( "f )" ); }
public:
	Vec2 xy() const{ return Vec2( x(), y() ); }
public:
	const glm::vec3& asGlm() const{ return _vec3; }
	operator const glm::vec3& () const{ return _vec3; }
#ifdef ODTJOLT
public:
	explicit Vec3( JPH::Vec3 jph )
		:Vec3( jph.GetX(), jph.GetY(), jph.GetZ() )
	{}
	JPH::Vec3 toJPH() const {
		return JPH::Vec3( x(), y(), z() );
	}
#endif
};


std::ostream &operator<<( std::ostream &os, const Vec3& v );
//Vec3 operator( const Vec3& v0, const Vec3& v1, const Vec3& v2 );
