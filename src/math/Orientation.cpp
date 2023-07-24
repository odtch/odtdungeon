#include "stdodt.h"
#include "Orientation.h"
#include "utils/Logger.h"

Orientation::Orientation()
	:_direction( Vec3::Forward )
	,_up( Vec3::Up )
	,_matrix_valid( false )
{
	_direction.normalize();
	_up.normalize();
}

Orientation::Orientation(const Vec3 &direction, const Vec3 &up)
	:_direction( direction )
	,_up( up )
	,_matrix_valid( false )
{
	_direction.normalize();
	_up.normalize();
}
Orientation::Orientation(const Orientation& src)
	:_direction( src._direction )
	,_up( src._up )
	,_matrix_valid( false )
{
	_direction.normalize();
	_up.normalize();
}
Orientation::Orientation( const Mat4 &src ){
	set( src );
}
const glm::quat Orientation::quat() const {
	int todo_cache;
	// https://stackoverflow.com/questions/23337151/trouble-converting-matrix-to-quaternion-and-back
	return glm::quat_cast( matrix().asGlm() );
}
//Orientation::Orientation( const glm::mat4& src )
//	:_position( src * vec4( 0, 0, 0, 1 ) )
//	,_rotation( glm::conjugate( glm::toQuat( src ) ) )
//	,_matrix_valid( false )
//{
//	matrix();
//	{
//		vec3 sx( src[0][0], src[1][0], src[2][0] );
//		vec3 sy( src[0][1], src[1][1], src[2][1] );
//		vec3 sz( src[0][2], src[1][2], src[2][2] );
//		vec3 scaling( glm::length( sx ), glm::length( sy ), glm::length( sz ) );
//	//	std::cout << "  t " << glm::to_string( _position ) << " \n";
//		std::cout << "  s " << glm::to_string( scaling ) << "\n";
//	//	std::cout.flush();
//	//	assert( isSame( scaling, vec3( 1, 1, 1 ), 0.0001f ) );
//	}
//	{
//		Mat4 m( src );
//		Vec3 tpos = m.map( Vec3::Null );
//		Vec3 tdir = m.mapNormal( Vec3::Forward );
//		Vec3 trig = m.mapNormal( Vec3::Right );
//		Vec3 tuup = m.mapNormal( Vec3::Up );
//		std::cout << " tpos:" << tpos.toString() << "\t" << glm::to_string( position() ) << "\n";
//		std::cout << " tdir:" << tdir.toString() << "\t" << glm::to_string( direction() ) << "\n";
//		std::cout << " trig:" << trig.toString() << "\t" << glm::to_string( right() ) << "\n";
//		std::cout << " tup :" << tuup.toString() << "\t" << glm::to_string( up() ) << "\n";

//		std::cout << "src= " << glm::to_string( src ) << "\n";
//		std::cout << "src= " << glm::to_string( _matrix ) << "\n";

//		assert( tpos.equals( Vec3( position() ) ) );
//		assert( tdir.equals( Vec3( direction() ) ) );
//		assert( trig.equals( Vec3( right() ) ) );
//		assert( tuup.equals( Vec3( up() ) ) );

////		vec3 new_pos = matrix() * vec4( 0, 0, 0, 1 );
////		vec3 new_pos_dir = matrix() * vec4( 0, 1, 0, 1 );
////		vec3 new_dir = Vec3( new_pos_dir - new_pos ).normalized().asGlm();

////		std::cout << "  src:" << glm::to_string( src_pos ) << "\tdir:" << glm::to_string( src_dir ) << "\n";
////		std::cout << "  new:" << glm::to_string( new_pos ) << "\tdir:" << glm::to_string( new_dir ) << "\n";
////		std::cout.flush();

////		assert( isSame( src_pos, new_pos, 0.0001f ) );
////		assert( isSame( src_dir, new_dir, 0.0001f ) );
//	//	vec3 ori_dir = src * vec4( 0, 1, 0, 1 );
//	//	vec3 new_dir = matrix() * vec4( 0, 1, 0, 1 );
//	//	assert( isSame( ori_dir, new_dir, 0.0001f ) );
//	}
//}
//void Orientation::setPosition(const Vec3& position){
//	_position = position;
//	_matrix_valid = false;
//}
//void Orientation::setRotation(const glm::quat& rotation){
//	_rotation = rotation;
//	_matrix_valid = false;
//}
const Mat4& Orientation::matrix() const{
	if( !_matrix_valid ){
		_matrix.setToIdentity();
		_right = Vec3::Cross( _direction, _up );
		//std::cout << "Orientation::matrix d " << _direction << ", " << _up << ", " << _right << "\n";
		_right.normalize();
		_matrix( 0, 0 ) = _right.x();
		_matrix( 1, 0 ) = _right.y();
		_matrix( 2, 0 ) = _right.z();
		_matrix( 0, 1 ) = _direction.x();
		_matrix( 1, 1 ) = _direction.y();
		_matrix( 2, 1 ) = _direction.z();
		_matrix( 0, 2 ) = _up.x();
		_matrix( 1, 2 ) = _up.y();
		_matrix( 2, 2 ) = _up.z();
		assert( _matrix( 3, 0 ) == 0.0f );
		assert( _matrix( 3, 1 ) == 0.0f );
		assert( _matrix( 3, 2 ) == 0.0f );
		assert( _matrix( 0, 3 ) == 0.0f );
		assert( _matrix( 1, 3 ) == 0.0f );
		assert( _matrix( 2, 3 ) == 0.0f );
		assert( _matrix( 3, 3 ) == 1.0f );
//		_direction = _rotation * vec3( 0, 1, 0 );
//		_right = _rotation * vec3( 1, 0, 0 );
//		_up = _rotation * vec3( 0, 0, 1 );
////std::cout << "  dir " <<  glm::to_string( _direction ) << "\n";
////		std::cout << "up " <<  glm::to_string( _up ) << "\n";
////		std::cout << "right " <<  glm::to_string( _right ) << "\n";
//		//_matrix = glm::toMat4( _rotation ) * glm::translate( glm::mat4( 1 ), _position );
//		_matrix = glm::translate( glm::mat4( 1 ), _position ) * glm::toMat4( _rotation );
//		// https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
////		glm::mat4 matinv = glm::inverse( _matrix );
////		_direction = Vec3(matinv[0][1], matinv[1][1], matinv[2][1]);
////		_right = Vec3(matinv[0][0], matinv[1][0], matinv[2][0]);
////		_up = vec3( matinv[0][2], matinv[1][2], matinv[2][2]);
//		Mat4 m( _matrix );
//		_direction = m.mapNormal( Vec3::Forward );
//		_right = m.mapNormal( Vec3::Right );
//		_up = m.mapNormal( Vec3::Up );
////		std::cout << "  dir " <<  glm::to_string( _direction ) << "\n";
////		std::cout << "up " <<  glm::to_string( _up ) << "\n";
////		std::cout << "right " <<  glm::to_string( _right ) << "\n";

		_matrix_valid = true;

		Vec3 nd = _matrix.map( Vec3::Forward );
		Vec3 nu = _matrix.map( Vec3::Up );
		Vec3 nr = _matrix.map( Vec3::Right );
//		std::cout << "\n------\n";
//		std::cout << "soll: " << _direction.toString() << "\t" << _right.toString()	<< "\t" << _up.toString() << "\n";
//		std::cout << " ist: " << nd.toString() << "\t" << nr.toString()	<< "\t" << nu.toString() << "\n";
		assert( _direction.equals( nd ) );
		assert( _up.equals( nu ) );
		assert( _right.equals( nr ) );
	}
	return _matrix;
}
void Orientation::set( const Vec3& direction, const Vec3& up ){
    ASSERT( fequals( direction.length(), 1, 0.001f ) );
    ASSERT( fequals( up.length(), 1, 0.001f ) );
    _direction = direction;
    _up = up;
    _matrix_valid = false;
}
void Orientation::set( const Mat4& matrix ){
	_direction = matrix.mapNormal( Vec3::Forward );
	_direction.normalize();
	_up = matrix.mapNormal( Vec3::Up );
	_up.normalize();
	_matrix_valid = false;
}
//void Orientation::set( const Vec3& position, const glm::quat& rotation ){
//	_position = position;
//	_rotation = rotation;
//	_matrix_valid = false;
//}
void Orientation::operator=( const Orientation& src ){
	_direction = src._direction;
	_up = src._up;
	_matrix_valid = false;
}
const Vec3& Orientation::axis( int a ) const {
	switch( a ){
	case 0:
		return right();
	case 1:
		return direction();
	case 2:
		return up();
	default:
		assert( false );
	}
}

//	_position = Orientation._position;
//	_rotation = Orientation._rotation;
//	_matrix_valid = false;
//}
//void Orientation::translate( const Vec3& offset ){
//	_position += offset;
//	_matrix_valid = false;
//}
//Orientation Orientation::translated(const Vec3 &offset) const{
//	return Orientation( _position + offset.asGlm(), _rotation );
//}
void Orientation::rotate( float angle, const Vec3& axis ){
//	std::cout << "Orientation rotate " << angle << " " << axis.toString();
//	std::cout << " pre " << toString();
	_matrix.setRotationAtAxis( angle, axis );
	_direction = _matrix.mapNormal( _direction );
	_up = _matrix.mapNormal( _up );
	_matrix_valid = false;
	//	std::cout << " post " << toString() << "\n";
}
Orientation Orientation::rotated(float angle, const Vec3& axis) const {
	Orientation copy( *this );
	copy.rotate( angle, axis );
	return copy;
}
void Orientation::rotate( const glm::quat& rotation ){
	_matrix.setRotation( rotation );
	_direction = _matrix.mapNormal( _direction );
	_up = _matrix.mapNormal( _up );
	_matrix_valid = false;
}
Orientation Orientation::calcRightAngled( const Orientation& o ) const {
	std::cout << "Orientation::calcRightAngled \n  " << toString() << ", \n  " << o.toString() << "\n  = ";
	Vec3 dir = direction();
	Vec3 up = this->up();
	float mdd = -11;
	float udd = -11;
	for( int i = 0; i < 6; i++ ){
		Vec3 v = vector( i );
		float dd = Vec3::Dot( o.direction(), v );
		if( dd > mdd ){
			dir = v;
			mdd = dd;
		}
		dd = Vec3::Dot( o.up(), v );
		if( dd > udd ){
			up = v;
			udd = dd;
		}
	}
	std::cout << dir << ", " << up << "\n";
	std::cout.flush();
	return Orientation( dir, up );
}
Vec3 Orientation::vector( int i ) const {
	switch( i ){
	case 0: return direction();
	case 1: return -direction();
	case 2: return right();
	case 3: return -right();
	case 4: return up();
	case 5: return -up();
	default:
		assert( false );
	}
}
bool Orientation::Equals( const Orientation& a, const Orientation& b, float max_dist ){
	if( max_dist < ( a.direction() - b.direction() ).length() )
		return false;
	if( max_dist < ( a.up() - b.up() ).length() )
		return false;
	if( max_dist < ( a.right() - b.right() ).length() )
		return false;
	return true;
}
Orientation Orientation::Simple( const Vec3& direction ){
	return Orientation( direction, Vec3::Cross( direction, Vec3( direction.y(), direction.z(), direction.x() ) ) );
}
Orientation Orientation::Interpolated( const Orientation& a, const Orientation& b, float f ){
	assert( 0 <= f && f <= 1.0f );
	const float fa = 1.0f - f;
	const float fb = f;
	Vec3 d = ( a.direction() * fa ) + ( b.direction() * fb );
	float dl = d.length();
	if( dl < 0.0001f ){
		logError( "Orientation::Interpolated d = 0" );
		d = a.direction();
	} else {
		d /= dl;
	}
	Vec3 u = ( a.up() * fa ) + ( b.up() * fb );
	float ul = u.length();
	if( ul < 0.0001f ){
		logError( "Orientation::Interpolated u = 0" );
		u = a.up();
	} else {
		u /= ul;
	}
	Vec3 r = Vec3::Cross( d, u );
	u = Vec3::Cross( d, -r );
	return Orientation( d, u );


}
std::string Orientation::toString() const {
	return std::string( "Ori(d=" ) + _direction.toString() + std::string( " u="   )  + _up.toString() + std::string( " r=" ) + right().toString() + std::string( ")" );
}

//	das geht ned
//	_rotation *= glm::angleAxis( -glm::radians( angle ), axis );
//	_matrix_valid = false;
//}

