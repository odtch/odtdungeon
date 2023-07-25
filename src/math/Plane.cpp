#include "stdodt.h"
#include "Plane.h"
#include "PosOri.h"
//#include "Math.h"
#include "Ray.h"

Plane::Plane()
	:_point( Vec3::Null )
	,_normal( Vec3::Up )
{
}
Plane::Plane( const Vec3& point, const Vec3& normal )
	:_point( point )
	,_normal( normal )
{
	assert( fequals( _normal.length2(), 1.0f, 0.002f ) );
}
Plane::Plane(const Plane& src)
	:_point( src.point() )
	,_normal( src.normal() )
{
}
void Plane::setPoint(const Vec3& point){
	_point = point;
}
void Plane::setNormal( const Vec3& normal ){
	assert( fequals( normal.length(), 1.0f, 0.0033f ) );
	_normal = normal;
}
void Plane::set( const Vec3& point, const Vec3& normal ){
	setPoint( point );
	setNormal( normal );
}
float Plane::distanceFrom( const Vec3& point ) const {
	return Vec3::Dot( point - _point, _normal ); // ???  / _normal.length();


}
bool Plane::intersectionDistance( const Ray& ray, float& distance ) const {
	float d = Vec3::Dot( _normal, ray.direction() );
	if( fequals( d, 0, 0.0001f ) ){
		// parallel
		return false;
	}
	distance = Vec3::Dot( _point - ray.origin(), _normal ) / d;
	return true;
}
bool Plane::intersectionPoint( const Ray& ray, Vec3& point ) const {
	float d;
	if( !intersectionDistance( ray, d ) )
		return false;
	point = ray.pointInDistance( d );
	return true;
}
std::string Plane::toString() const {
	return std::string( "Plane( " + _point.toString() + ", " + _normal.toString() + " )" );
}
