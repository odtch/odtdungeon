#include "stdodt.h"
#include "PosOri.h"
//#include "Math.h"
//#include "utils/File.h"
#include "utils/Logger.h"

PosOri::PosOri()
	:_position( 0, 0, 0 )
	,_matrix_valid( false )
{
}
PosOri::PosOri( const Vec3& position, const Orientation& orientation )
	:_position( position )
	,_orientation( orientation )
	,_matrix_valid( false )
{
	ASSERT( !_position.isNan() );
}
PosOri::PosOri(const Vec3 &position, const Vec3 &direction, const Vec3 &up )
	:_position( position )
	,_orientation( direction, up )
	,_matrix_valid( false )
{
}
PosOri::PosOri(const PosOri& src)
	:_position( src._position )
	,_orientation( src._orientation )
	,_matrix_valid( false )
{
}
PosOri::PosOri( const Mat4 &src ){
	set( src );
}
//PosOri::PosOri( const glm::mat4& src )
//	:_position( src * vec4( 0, 0, 0, 1 ) )
//	,_orientation( src )
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
void PosOri::setPosition(const Vec3& position){
	_position = position;
	_matrix_valid = false;
}
void PosOri::setOrientation( const Orientation& orientation ){
	_orientation = orientation;
	_matrix_valid = false;
}
const Mat4& PosOri::matrix() const{
	if( !_matrix_valid ){
		_matrix.setTranslation( _position );
		_matrix *= _orientation.matrix();
//		assert( false );
////		_direction = _orientation * vec3( 0, 1, 0 );
////		_right = _orientation * vec3( 1, 0, 0 );
////		_up = _orientation * vec3( 0, 0, 1 );
//////std::cout << "  dir " <<  glm::to_string( _direction ) << "\n";
//////		std::cout << "up " <<  glm::to_string( _up ) << "\n";
//////		std::cout << "right " <<  glm::to_string( _right ) << "\n";
////		//_matrix = glm::toMat4( _orientation ) * glm::translate( glm::mat4( 1 ), _position );
////		_matrix = glm::translate( glm::mat4( 1 ), _position ) * glm::toMat4( _orientation );
////		// https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
//////		glm::mat4 matinv = glm::inverse( _matrix );
//////		_direction = glm::vec3(matinv[0][1], matinv[1][1], matinv[2][1]);
//////		_right = glm::vec3(matinv[0][0], matinv[1][0], matinv[2][0]);
//////		_up = vec3( matinv[0][2], matinv[1][2], matinv[2][2]);
////		Mat4 m( _matrix );
////		_direction = m.mapNormal( Vec3::Forward );
////		_right = m.mapNormal( Vec3::Right );
////		_up = m.mapNormal( Vec3::Up );
//////		std::cout << "  dir " <<  glm::to_string( _direction ) << "\n";
//////		std::cout << "up " <<  glm::to_string( _up ) << "\n";
//////		std::cout << "right " <<  glm::to_string( _right ) << "\n";
		_matrix_valid = true;
	}
	return _matrix;
}
void PosOri::set( const Vec3& position, const Vec3& direction, const Vec3& up ){
	ASSERT( !position.isNan() );
	_position = position;
	_orientation.set( direction, up );
}
void PosOri::set( const Vec3& position, const Orientation& orientation ){
	ASSERT( !position.isNan() );
	_position = position;
	_orientation = orientation;
}
void PosOri::set( const Mat4& matrix ){
	_position = matrix.map( Vec3::Null );
	_orientation.set( matrix );
	_matrix_valid = false;
}
//void PosOri::set( const glm::vec3& position, const glm::quat& orientation ){
//	_position = position;
//	_orientation = orientation;
//	_matrix_valid = false;
//}
void PosOri::operator=( const PosOri& posori ){
	_position = posori._position;
	_orientation = posori._orientation;
	_matrix_valid = false;
}
void PosOri::translate( const Vec3& offset ){
	_position += offset;
	_matrix_valid = false;
}
PosOri PosOri::translated( const Vec3& offset) const{
	return PosOri( _position + offset, _orientation );
}
void PosOri::translateLocal( const Vec3& offset ){
	translate( direction() * offset.y() + right() * offset.x() + up() * offset.z() );
}
PosOri PosOri::translatedLocal(const Vec3& offset) const{
	return translated( direction() * offset.y() + right() * offset.x() + up() * offset.z() );
}
void PosOri::rotate( float angle, const Vec3& axis ){
	_orientation.rotate( angle, axis );
	_matrix_valid = false;
}
PosOri PosOri::rotated(float angle, const Vec3 &axis) const {
	PosOri p( *this );
	p.rotate( angle, axis );
	return p;
}
void PosOri::rotate( const glm::quat& rotation ){
	_orientation.rotate( rotation );
	_matrix_valid = false;
}
PosOri PosOri::rotated( const glm::quat& rotation ) const {
	PosOri p( *this );
	p.rotate( rotation );
	return p;
}

void PosOri::setZto0()
{
	setPosition( Vec3( position().x(), position().y(), 0 ) );
	Vec3 dir( direction().x(), direction().y(), 0 );
	setOrientation( Orientation( dir, Vec3::Up ) );
}
//void PosOri::save( BinaryFileWriter& writer ) const{
//	writer.write_uint8( 'O' );
//	writer.write_vec3( position() );
//	writer.write_vec3( direction() );
//	writer.write_vec3( up() );
//}
//void PosOri::load( BinaryFileReader& reader ){
//	if( reader.read_uint8() != 'O' ){
//		assert( false );
//	}
//	Vec3 pos = Vec3( reader.read_vec3() );
//	Vec3 dir = Vec3( reader.read_vec3() );
//	Vec3 up  = Vec3( reader.read_vec3() );
//	set( pos, Orientation( dir, up ) );
//}
bool PosOri::Equals( const PosOri& a, const PosOri& b, float max_pos_dist, float max_ori_dist ){
	if( max_pos_dist < ( a.position() - b.position() ).length() )
		return false;
	if( !Orientation::Equals( a.orientation(), b.orientation(), max_ori_dist ) )
		return false;
	return true;
}
PosOri PosOri::Interpolated( const PosOri &a, const PosOri &b, float f ){
	assert( 0 <= f && f <= 1.0f );
	const float fa = 1.0f - f;
	const float fb = f;
	return PosOri( ( a.position() * fa ) + ( b.position() * fb ), Orientation::Interpolated( a.orientation(), b.orientation(), f ) );
	/*
	const float df = 10;
	Vec3 p = ( a.position() * fa ) + ( b.position() * fb );
	Vec3 t = ( ( a.position() + a.direction() * df ) * fa ) + ( ( b.position() + b.direction() * df ) * fb );
	Vec3 u = ( ( a.position() + a.up() * df ) * fa ) + ( ( b.position() + b.up() * df ) * fb );
	Vec3 d = ( t - p );
	float dl = d.length();
	if( dl < 0.0001f ){
		logDebug( "PosOri::Interpolated error d" );
		d = a.direction();
	} else {
		d /= dl;
	}
	u = u - p;
	float ul = u.length();
	if( ul < 0.0001f ){
		logDebug( "PosOri::Interpolated error u" );
		u = a.up();
	} else

	}

	Vec3 d = ( a.direction() * fa + b.direction() * fb ).normalized();
	Vec3 u = ( a.up() * fa + b.up() * fb ).normalized();
//	Vec3 r = ( a.right() * fa + b.right() * fb ).normalized();
//	u = Vec3::Cross( r, d );
	return PosOri( p, d, u );
	/*
	const glm::quat oa = glm::toQuat( a.orientation().matrix().asGlm() );
	const glm::quat ob = glm::toQuat( b.orientation().matrix().asGlm() );
	const glm::quat of = glm::slerp( oa, ob, fb );
	Mat4 om( glm::toMat4( of ) );
//	Mat4 m( a.orientation().matrix().asGlm() * fa + b.orientation().matrix().asGlm() * fb );
	return PosOri( p, Orientation( om ) );
	*/
}
PosOri PosOri::AbsoluteToRelative( const PosOri& parentposori, const PosOri& absoluteposori ){
	return AbsoluteToRelative( parentposori.matrix(), absoluteposori.matrix() );
}
PosOri PosOri::AbsoluteToRelative( const Mat4& parentmatrix, const Mat4& absolutematrix ){
	Mat4 relativematrix = parentmatrix.inverted() * absolutematrix;
//#ifdef ODTDEBUG
//	PosOri relposori( relativematrix );
//	logDebug( "PosOri::AbsoluteToRelative parent=", parentmatrix.map( Vec3::Null ), "child=", absolutematrix.map( Vec3::Null ), "rel=", relposori.position() );
//	ASSERT( ( absolutematrix.map( Vec3::Null ) - relposori.position() ).length() < 0.01f );
//#endif
	return PosOri( relativematrix );
}
#ifdef ODTJOLT
void PosOri::set( JPH::Mat44 mat44 ){
	float & d = mat44(0,0);
	Mat4 t;
	float& td = t(0,0);
	const float* dp = &d;
	float* tp = &td;
	for( int i = 0; i < 4 * 4; i++ ){
		(*tp) = (*dp);
		tp++; dp++;
	}
	set( t );
}
#endif
std::string PosOri::toString() const {
	return std::string( "Pos=" ) + _position.toString() + std::string( " Ori="  ) + _orientation.toString();
}
std::string PosOri::toCode() const{
	return std::string( "PosOri( " ) + _position.toString() + std::string( ", "  )
		+ _orientation.direction().toString() + std::string( ", "  )
		+ _orientation.up().toString() + std::string( " )"  );
}

