#pragma once

#include "MathCore.h"

struct Vec4 {
public:
    static const Vec4 Null;
    static const Vec4 One;
private:
    union {
        struct { float x; float y; float z; float w; } data;
        glm::vec4 _vec4;
	};
public:
    explicit Vec4()
        :_vec4( 0, 0, 0, 0 )
	{}
    explicit Vec4( float x, float y, float z, float w )
        :_vec4( x, y, z, w )
	{
		ASSERT_VALID();
	}
    Vec4( const Vec4& vec )
        :_vec4( vec._vec4 )
	{
		ASSERT_VALID();
	}
    explicit Vec4( const glm::vec4& vec )
        :_vec4( vec )
	{
		ASSERT_VALID();
	}
public:
    float x() const{ return _vec4.x; }
    float y() const{ return _vec4.y; }
    float z() const{ return _vec4.z; }
    float w() const{ return _vec4.w; }
    float& x(){ return _vec4.x; }
    float& y(){ return _vec4.y; }
    float& z(){ return _vec4.z; }
    float& w(){ return _vec4.w; }
public:
    void setX( float x ){ _vec4.x = x; ASSERT_VALID(); }
    void setY( float y ){ _vec4.y = y; ASSERT_VALID(); }
    void setZ( float z ){ _vec4.z = z; ASSERT_VALID(); }
    void setW( float w ){ _vec4.w = w; ASSERT_VALID(); }
public:
    void set( float x, float y, float z, float w ){
        _vec4.x = x; _vec4.y = y; _vec4.z = z; _vec4.w = w;
	}
	void clear(){
        set( 0, 0, 0, 0 );
	}
public:
    float length() const{ return glm::length( _vec4 ); }
    float length2() const{ return glm::length2( _vec4 ); }
public:
    void operator *=( float s ){ _vec4 *= s; ASSERT_VALID(); }
    Vec4 operator *( float s ) const{ return Vec4( _vec4 * s ); }
    Vec4 operator /( float s ) const{ ASSERT( s != 0 ); return Vec4( _vec4 / s ); }
    void operator /=( float s ){ _vec4 /= s; ASSERT_VALID(); }
    Vec4 operator -() const { return Vec4( -x(), -y(), -z(), -w() ); }
    Vec4 operator -( const Vec4& o ) const{ return Vec4( x() - o.x(), y() - o.y(), z() - o.z(), w() - o.w() );}
    void operator -=( const Vec4& o ){ _vec4 -= o.asGlm(); ASSERT_VALID(); }
    Vec4 operator +( const Vec4& o ) const{ return Vec4( x() + o.x(), y() + o.y(), z() + o.z(), w() + o.w() );}
    void operator +=( const Vec4& o ){ _vec4 += o.asGlm(); ASSERT_VALID(); }
    void operator =( const Vec4& o ){ _vec4 = o.asGlm(); ASSERT_VALID(); }
    void operator =( const glm::vec4& o ){ _vec4 = o; ASSERT_VALID(); }
    Vec4 operator,( const Vec4& v1 ) = delete;
public:
	void normalize(){
        //std::cout << "Vec4 " << toString() << ".normalize ";
		float l = length();
		while( 100000.0f < l ){
            std::cout << "Vec4::normalize " << toString() << " is too long\n";
			(*this) *= 0.01f;
			l = length();
		}
		//float l = std::sqrt( l2 );
		if( l < 0.0001f ){
            std::cout << "Vec4::normalize " << toString() << " is too small\n";
			//l = 0.0001f;
			//assert( false );
		}
		//std::cout << "n " << l2 << ", " << l << " " << length() << "\n";
        //_vec4 = glm::normalize( _vec4 );
        _vec4 /= l;
		//std::cout << "l=" << length() << "\n"; std::cout.flush();
		ASSERT( std::abs( length() - 1.0f ) < 0.001f );
//		std::cout << " is " << toString() << "\n";
//		std::cout.flush();
		//ASSERT_VALID();
		if( !isValid() ){
            std::cout << "Vec4::normalize " << toString() << " is not valid\n";
			std::cout.flush();
            _vec4 = glm::vec4( 0, 0, 0, 1 );
		}
	}
    Vec4 normalized() const{
        Vec4 n( *this );
		n.normalize();
		return n;
	}
public:
	float& operator[]( int i ){
        assert( 0 <= i && i < 4 );
        return _vec4[i];
	}
	float operator[]( int i ) const {
        assert( 0 <= i && i < 4 );
        return _vec4[i];
	}
//public:
//    Vec4 xed( float v ) const{ return Vec4(  v , y(), z() ); }
//    Vec4 yed( float v ) const{ return Vec4( x(),  v , z() ); }
//    Vec4 zed( float v ) const{ return Vec4( x(), y(),  v  ); }
public:
	int highestIndex() const{
        if( _vec4[0] > _vec4[1] && _vec4[0] > _vec4[2] )
			return 0;
        if( _vec4[1] > _vec4[2] )
			return 1;
		return 2;
	}
	float sum() const{ return data.x + data.y + data.z; }
public:
	void ASSERT_VALID() const {
		if( !isValid() ){
            std::cout << "Vec4 not valid " << toString() << "\n";
			std::cout.flush();
			assert( false );
		}
	}
public:
    bool equals( const Vec4& v, float maxDiff = 0.0001f ) const {
		if( maxDiff <= glm::abs( v.x() - x() ) )return false;
        if( maxDiff <= glm::abs( v.y() - y() ) )return false;
        if( maxDiff <= glm::abs( v.z() - z() ) )return false;
        if( maxDiff <= glm::abs( v.w() - w() ) )return false;
        return true;
	}
	bool isNan() const{
        if( isnanf( _vec4.x ) )return true;
        if( isnanf( _vec4.y ) )return true;
        if( isnanf( _vec4.z ) )return true;
        if( isnanf( _vec4.w ) )return true;
        return false;
	}
	bool isValid() const{
		if( isNan() )return false;
        if( isinff( _vec4.x ) )return false;
        if( isinff( _vec4.y ) )return false;
        if( isinff( _vec4.z ) )return false;
        if( isinff( _vec4.w ) )return false;
        return true;
	}
public:
    static Vec4 Lerp( const Vec4& a, const Vec4& b, float f );
public:
    glm::vec4 toVec40() const{ return glm::vec4( data.x, data.y, data.z, 0 ); }
public:
    std::string toString() const{ return std::string( "Vec4( " ) + std::to_string( x() ) + std::string( "f, " ) + std::to_string( y() ) + std::string( "f, " ) + std::to_string( z() ) + std::string( "f, " ) + std::to_string( w() ) + std::string( "f )" ); }
public:
    const glm::vec4& asGlm() const{ return _vec4; }
    operator const glm::vec4& () const{ return _vec4; }
};


std::ostream &operator<<( std::ostream &os, const Vec4& v );
//Vec4 operator( const Vec4& v0, const Vec4& v1, const Vec4& v2 );
