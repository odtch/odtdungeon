#include "stdodt.h"
#include "Ray.h"
#include "PosOri.h"
//#include "Math.h"

Ray::Ray()
	:_origin( Vec3::Null )
	,_direction( Vec3::Forward )
{
}
Ray::Ray( const Vec3& origin, const Vec3& direction )
	:_origin( origin )
	,_direction( direction )
{
	//std::cout << "Ray( " << origin.toString() << ", " << direction.toString() << "\n";
	assert( fequals( _direction.length2(), 1.0f, 0.0001f ) );
}
Ray::Ray(const Ray& src)
	:_origin( src.origin() )
	,_direction( src.direction() )
{
}
Ray::Ray(const PosOri& src)
	:_origin( src.position() )
	,_direction( src.direction() )
{
}
void Ray::setOrigin(const Vec3& origin){
	_origin = origin;
}
void Ray::setDirection( const Vec3& direction ){
	//std::cout << "Ray::setDirection " << direction.toString() << "\n";
	if( !fequals( direction.length2(), 1.0f, 0.001f ) ){
		std::cout << "Ray::setDirection " << direction.toString() << "\n";
		assert( false );
	}
	_direction = direction;
}
void Ray::set( const Vec3& origin, const Vec3& direction ){
	//std::cout << "Ray::set " << origin.toString() << ", " << direction.toString() << "\n";
	setOrigin( origin );
	setDirection( direction );
}
void Ray::set(const Ray& ray){
	_origin = ray._origin;
	_direction = ray._direction;
}
void Ray::operator=( const Ray& ray ){
	set( ray );
}
Vec3 Ray::pointInDistance(float distance) const{
	ASSERT( !std::isnan( distance ) );
	//assert( 0 < distance );
	return _origin + _direction * distance;
}
float Ray::closestPointDistance(const Vec3& point) const {
	return Vec3::Dot( point - _origin, _direction );
}
std::string Ray::toString() const {
	return std::string( "Ray( " + _origin.toString() + ", " + _direction.toString() + " )" );
}

std::ostream &operator<<( std::ostream &os, const Ray& ray ){
	os << ray.toString();
	return os;
}
