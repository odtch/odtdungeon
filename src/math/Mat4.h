#pragma once

#include "MathCore.h"
#include "Vec3.h"

struct Mat4 {
private:
	glm::mat4 _mat4;
public:
	explicit Mat4()
		:_mat4( 1.0f )
	{}
	explicit Mat4( const glm::mat4& mat )
		:_mat4( mat )
	{}
public:
	Vec3 map( const Vec3& vec ) const{
		return Vec3( _mat4 * glm::vec4( vec.asGlm(), 1.0f ) );
	}
	Vec3 mapNormal( const Vec3& vector ) const{
		//return Vec3( _mat4 * glm::vec4( vec.asGlm(), 1.0f ) );
		// qmatrix4x4::mapVector:
		Vec3 qs(vector.x() * _mat4[0][0] +
						 vector.y() * _mat4[1][0] +
						 vector.z() * _mat4[2][0],
						 vector.x() * _mat4[0][1] +
						 vector.y() * _mat4[1][1] +
						 vector.z() * _mat4[2][1],
						 vector.x() * _mat4[0][2] +
						 vector.y() * _mat4[1][2] +
						 vector.z() * _mat4[2][2]);
		if( !qs.isValid() ){
			std::cout << "mapNormal " << vector.toString() << " q:" << qs.toString() << "\n"; std::cout.flush();
		}
		// " t:" << t.toString() <<
		qs.normalize();
//		Vec3 t = map( vector ) - map( Vec3::Null );
//		assert( t.equals( qs ) );
//		assert( abs( t.length2() -1.0f ) < 0.0001f );
//		assert( abs( qs.length2() -1.0f ) < 0.0001f );
		return qs;
	}
public:
	void setToIdentity(){
		_mat4 = glm::mat4( 1.0f );
	}
	void setTranslation( const Vec3& position ){
		_mat4 = glm::translate( glm::mat4( 1.0f ), position.asGlm() );
	}
	static Mat4 Translation( const Vec3& position ){
		return Mat4( glm::translate( glm::mat4( 1.0f ), position.asGlm() ) );
	}
	static Mat4 RotationAtAxis( float degrees, const Vec3& axis ){
		Mat4 m;
		m.setRotationAtAxis( degrees, axis );
		return m;
	}
	void setRotationAtAxis( float degrees, const Vec3& axis ){
		assert( std::abs( 1.0f - axis.length2() ) <= 0.00001f );
		float angle = //degrees; //
				- glm::radians( degrees );
				//degrees * ( 3.141592f / 180.0f );

		_mat4 = glm::toMat4( glm::angleAxis( angle, axis.asGlm() ) );
	}
	void setRotation( const glm::quat& rotation ){
		_mat4 = glm::mat4_cast( rotation );
	}
	static Mat4 Rotation( const glm::quat& rotation ){
		Mat4 m;
		m.setRotation( rotation );
		return m;
	}
	glm::quat getRotation() const{
		return glm::quat_cast( _mat4 );
	}
	static Mat4 Scale( float s ){
		return Mat4( glm::scale( glm::mat4( 1.0f ), glm::vec3( s, s, s ) ) );
	}
	void setScale( float s ){
		_mat4 = glm::scale( glm::mat4( 1.0f ), glm::vec3( s, s, s ) );
	}
	Mat4 inverted() const{
		return Mat4( glm::inverse( asGlm() ) );
	}
	void inverse(){
		_mat4 = glm::inverse( _mat4 );
	}
public:
	Mat4 operator *( const Mat4& mat ) const {
		return Mat4( _mat4 * mat.asGlm() );
	}
	void operator *=( const Mat4& mat ){
		_mat4 *= mat.asGlm();
	}
public:
	float& operator()( int c, int r ){ return _mat4[r][c]; }
	const float& operator()( int c, int r ) const { return _mat4[r][c]; }
public:
//	void load( BinaryFileReader& reader );
//	void write( BinaryFileWriter& writer ) const;
public:
	const glm::mat4& asGlm() const{ return _mat4; }
	glm::mat4& asGlm(){ return _mat4; }
	glm::quat toGlmQuat() const;
#ifdef ODTJOLT
public:
//	explicit Vec3( JPH::Vec3 jph )
//		:Vec3( jph.GetX(), jph.GetY(), jph.GetZ() )
//	{}
	JPH::Mat44 toJPH() const {
		JPH::Mat44 jph = JPH::Mat44::sZero();
		for( int r = 0; r < 4; r++ ){
			for( int c = 0; c < 4; c++ ){
				jph(r,c) = _mat4[c][r];
			}
		}
		return jph;
	}
#endif
public:
	const std::string toString() const{
		std::string s( "Mat4( " );
		for( int x = 0; x < 4; x++ ){
			for( int y = 0; y < 4; y++ ){
				float v = _mat4[x][y];
				s += std::to_string( v );
				s += std::string( "f" );
				if( y < 4 ){
					s += std::string( ", " );
				}
			}
			if( x < 4 ){
				s += std::string( " | " );
			}
		}
		s += std::string( " )" );
		return s;
	}
};
