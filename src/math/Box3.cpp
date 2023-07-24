#include "stdodt.h"
#include "Box3.h"

Box3 Box3::WithRadius( const Vec3& radius ){
	return FromCenterWithRadius( Vec3::Null, radius );
}
Box3 Box3::FromCenterWithRadius( const Vec3& position, const Vec3& radius ){
    Box3 box;
	box.unite( position + radius );
	box.unite( position - radius );
	return box;
}
Box3::Box3()
	:_empty( true )
{
}
Box3::Box3(const Box3& src)
	:_empty( src._empty )
	,_min( src._min )
	,_max( src._max )
{
}
Box3::Box3( const Vec3 &v1, const Vec3 &v2 ){
	unite( v1 );
	unite( v2 );
}
void Box3::reset(){
	if( !_empty ){
		_empty = true;
		_min = Vec3::Null;
		_max = Vec3::Null;
	}
}
void Box3::unite(const Box3 &b){
	unite( b.min() );
	unite( b.max() );
}
void Box3::unite( const Vec3& vec ){
	if( _empty ){
		_empty = false;
		_min = vec;
		_max = vec;
	} else {
		_min.x() = ( _min.x() < vec.x() ? _min.x() : vec.x() );
		_min.y() = ( _min.y() < vec.y() ? _min.y() : vec.y() );
		_min.z() = ( _min.z() < vec.z() ? _min.z() : vec.z() );
		_max.x() = ( _max.x() > vec.x() ? _max.x() : vec.x() );
		_max.y() = ( _max.y() > vec.y() ? _max.y() : vec.y() );
		_max.z() = ( _max.z() > vec.z() ? _max.z() : vec.z() );
	}
}
void Box3::unite(const glm::vec3 &vec){
	unite( Vec3( vec ) );
}
Vec3 Box3::center() const {
	return Vec3( ( _min.x() + _max.x() ) / 2,
				 ( _min.y() + _max.y() ) / 2,
				 ( _min.z() + _max.z() ) / 2 );
}
Vec3 Box3::size() const {
	return Vec3( ( _max.x() - _min.x() ),
				 ( _max.y() - _min.y() ),
				 ( _max.z() - _min.z() ) );
}
Vec3 Box3::radius() const {
	return size() * 0.5f;
}

std::string Box3::toString() const
{
    return std::string( "Box3( " ) + _min.toString() + std::string( " to " ) + _max.toString() + std::string( " c=" ) + center().toString() + std::string( " )" );
}


std::ostream &operator<<( std::ostream &os, const Box3& v ){
	return os << v.toString();
}
