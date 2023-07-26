#include "stdodt.h"

#include "Camera.h"
#include "Plane.h"
#include "Ray.h"
#include "utils/Logger.h"

Camera::Camera(){
	_fov = vec2( 45, 45 );
}
Camera::~Camera(){
}
void Camera::setPosOri( const PosOri& posori ){
	//logDebug( "Camera::setPosOri", posori.position(), posori.direction() );
	_posori = posori;
}
void Camera::translate( const Vec3 &offset ){
	//logDebug( "Camera::translate", offset );
	_posori.translate( offset );
}
void Camera::rotate( float angle, const Vec3& axis ){
	_posori.rotate( angle, axis );
}
void Camera::setViewCenter( const Vec3& view_center ){
	_view_center = view_center;
}
void Camera::setFov( const glm::vec2& fov ){
	assert( 1 < fov.x && fov.x < 170 );
	assert( 1 < fov.y && fov.y < 170 );
	_fov = fov;
}
//void Camera::setProjectionOrthographic( const Rect2i& viewportSize ){
//	_projection = Projection::Orthographic;
//	_orthographic_viewport_size = viewportSize;
//}
void Camera::recalcTest(){
	if( _projection == Projection::Perspective ){
		_near_plane_center = position() + direction() * 10;
		//std::cout << "Camera recalcTest " << position() << "    " << direction() << "   " << _near_plane_center << "    " << _fov.x << " " << _fov.y << " \n";
		Plane near_plane(  _near_plane_center, direction() );
		//std::cout << "fov " << Vec2( _fov ).toString() << "\n";
		if( !near_plane.intersectionPoint( Ray( position(), ( Mat4::RotationAtAxis( -_fov.x / 2, orientation().up() ) * Mat4::RotationAtAxis( -_fov.y / 2, orientation().right() ) ) .mapNormal( direction() ) ), _near_plane_lt ) ){ assert( false ); }
		if( !near_plane.intersectionPoint( Ray( position(), ( Mat4::RotationAtAxis( +_fov.x / 2, orientation().up() ) * Mat4::RotationAtAxis( -_fov.y / 2, orientation().right() ) ) .mapNormal( direction() ) ), _near_plane_rt ) ){ assert( false ); }
		if( !near_plane.intersectionPoint( Ray( position(), ( Mat4::RotationAtAxis( +_fov.x / 2, orientation().up() ) * Mat4::RotationAtAxis(  _fov.y / 2, orientation().right() ) ) .mapNormal( direction() ) ), _near_plane_rb ) ){ assert( false ); }
		if( !near_plane.intersectionPoint( Ray( position(), ( Mat4::RotationAtAxis( -_fov.x / 2, orientation().up() ) * Mat4::RotationAtAxis(  _fov.y / 2, orientation().right() ) ) .mapNormal( direction() ) ), _near_plane_lb ) ){ assert( false ); }
	} else if( _projection == Projection::Orthographic ){
		assert( false );
////		_near_plane_center = position() - direction() * 10; int todo_war_mal_minus;
////		_near_plane_lt = _near_plane_center + orientation().right() * _orthographic_viewport_size.width() * -0.5f + orientation().up() * _orthographic_viewport_size.height() * -0.5f;
////		_near_plane_rt = _near_plane_center + orientation().right() * _orthographic_viewport_size.width() *  0.5f + orientation().up() * _orthographic_viewport_size.height() * -0.5f;
////		_near_plane_rb = _near_plane_center + orientation().right() * _orthographic_viewport_size.width() *  0.5f + orientation().up() * _orthographic_viewport_size.height() *  0.5f;
////		_near_plane_lb = _near_plane_center + orientation().right() * _orthographic_viewport_size.width() * -0.5f + orientation().up() * _orthographic_viewport_size.height() *  0.5f;
//		auto to_3d = [&]( float fx, float fy ){
//			return position()
//					+ direction() * -100
//					+ orientation().right() * fx
//					+ orientation().up() * fy;
//		};
//		_near_plane_center = to_3d( _orthographic_viewport_size.center().x, _orthographic_viewport_size.center().y );
//		_near_plane_lt = to_3d( _orthographic_viewport_size.lt.x, _orthographic_viewport_size.lt.y );
//		_near_plane_rt = to_3d( _orthographic_viewport_size.rb.x, _orthographic_viewport_size.lt.y );
//		_near_plane_rb = to_3d( _orthographic_viewport_size.rb.x, _orthographic_viewport_size.rb.y );
//		_near_plane_lb = to_3d( _orthographic_viewport_size.lt.x, _orthographic_viewport_size.rb.y );
//		//std::cout << "camera " << *this << "\n";
	} else {
		assert( false );
	}
}
Vec3 Camera::point_on_near_plane_01( const Vec2& factor ) const {
	assert( 0 <= factor.x() && factor.x() <= 1 );
	assert( 0 <= factor.y() && factor.y() <= 1 );
	return ( _near_plane_lt * ( 1.0f - factor.x() ) + _near_plane_rt * factor.x() ) * ( 1.0f - factor.y() )
			+ ( _near_plane_lb * ( 1.0f - factor.x() ) + _near_plane_rb * factor.x() ) * factor.y();
}
void Camera::recalcMatrices( const glm::vec2& screen_size ){
	assert( _projection == Projection::Perspective );
	PosOri po;
	po = posori();
	_view_matrix = Mat4( glm::lookAtRH( po.position().asGlm(), po.position().asGlm() + po.direction().asGlm(), po.up().asGlm() ) );
	glm::mat4 pm = glm::perspective(
//										  glm::radians(45.0f),
			glm::radians( fov().y ),
			screen_size.x / screen_size.y,
//			glm::radians( viewer.camera().fov().y ),
			0.1f, 1200.0f
	);
	pm[1][1] *= -1;
	_projection_matrix = Mat4( pm );
}
void Camera::set( const Camera& camera ){
	//logDebug( "Camera::set", camera._posori.position() );
	_posori = camera._posori;
	_view_center = camera._view_center;
	_projection = camera._projection;
	//_orthographic_viewport_size = camera._orthographic_viewport_size;
	_fov = camera._fov;
	_view_matrix = camera._view_matrix;
	_projection_matrix = camera._projection_matrix;
	_near_plane_center = camera._near_plane_center;
	_near_plane_lt = camera._near_plane_lt;
	_near_plane_rt = camera._near_plane_rt;
	_near_plane_rb = camera._near_plane_rb;
	_near_plane_lb = camera._near_plane_lb;
}
void Camera::operator=( const Camera& camera ){
	set( camera );
}
/*
void Camera::set( const Camera& camera ){
	_posori = camera._posori;
	_view_center = camera._view_center;
	_projection = camera._projection;
	_orthographic_viewport_size = camera._orthographic_viewport_size;
	_fov = camera._fov;
	_view_matrix = camera._view_matrix;
	_projection_matrix = camera._projection_matrix;
	_near_plane_center = camera._near_plane_center;
	_near_plane_lt = camera._near_plane_lt;
	_near_plane_rt = camera._near_plane_rt;
	_near_plane_rb = camera._near_plane_rb;
	_near_plane_lb = camera._near_plane_lb;
	_area = camera._area;
}
*/
//void Camera::setPosOri( const PosOri& posori ){
//	_posori = posori;
//}
//void Camera::lookAt( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up ){
//	vec3 direction = center - eye;
//	float l = glm::length( direction );
//	assert( 0 < l );
//	direction /= l;
//	assert( std::abs( glm::length( up ) -1.0f ) < 0.001f );
//	glm::quat rot = quatLookAt( direction, up );
//	_posori.set( eye, rot );
//}


std::ostream &operator<<( std::ostream& os, const Camera& camera ){
	return os << "Camera( pos=" << camera.position()
			  << ", ori=" << camera.orientation().toString()
			  << ", viewcenter=" << camera.view_center()
			  << ", fov=" << glm::to_string( camera.fov() )
			  << ", projection=" << camera.projection()
			  << ", npc=" << camera.near_plane_center()
			  << " lt=" << camera.near_plane_lt()
			  << " rb=" << camera.near_plane_rb()
			  << ");";


}
