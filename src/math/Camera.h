#pragma once

#include "MathCore.h"
#include "PosOri.h"
//#include "Math2i.h"
#include "Vec2.h"
#include "Vec3.h"

class Camera
{
private:
	PosOri _posori;
private:
	Vec3 _view_center;
private:
	Projection::Type _projection = Projection::Perspective;
private:
//	Rect2i _orthographic_viewport_size;
private:
	vec2 _fov;
private:
	Mat4 _view_matrix;
	Mat4 _projection_matrix;
private:
	Vec3 _near_plane_center;
	Vec3 _near_plane_lt;
	Vec3 _near_plane_rt;
	Vec3 _near_plane_rb;
	Vec3 _near_plane_lb;
public:
	explicit Camera();
	virtual ~Camera();
public:
	//const glm::mat4& matrix() const { return _posori.matrix(); };
	//const glm::mat4& matrix() const { return _matrix; };
public:
	const Vec3& position() const{ return _posori.position(); }
	const Orientation& orientation() const{ return _posori.orientation(); }
	const PosOri& posori() const{ return _posori; }
	void setPosOri( const PosOri& posori );
public:
	const Vec3& direction() const { return _posori.direction(); }
	const Vec3& right() const { return _posori.right(); }
	const Vec3& up() const { return _posori.up(); }
public:
	void translate( const Vec3& offset );
	void rotate( float angle, const Vec3& axis );
public:
	const Vec3& view_center() const{ return _view_center; }
	void setViewCenter( const Vec3& view_center );
public:
	const vec2& fov() const{ return _fov; }
	void setFov( const vec2& fov );
public:
	Projection::Type projection() const{ return _projection; }
public:
//	void setProjectionOrthographic( const Rect2i& viewportSize );
public:
	void recalcTest();
public:
	const Vec3& near_plane_center() const{ return _near_plane_center; }
	const Vec3& near_plane_lt() const{ return _near_plane_lt; }
	const Vec3& near_plane_rt() const{ return _near_plane_rt; }
	const Vec3& near_plane_rb() const{ return _near_plane_rb; }
	const Vec3& near_plane_lb() const{ return _near_plane_lb; }
	Vec3 point_on_near_plane_01( const Vec2& factor ) const;
public:
	const Mat4& view_matrix() const{ return _view_matrix; }
	const Mat4& projection_matrix() const{ return _projection_matrix; }
	void recalcMatrices( const vec2& screen_size );
public:
	void set( const Camera& camera );
	void operator=( const Camera& camera );
//public:
//	void lookAt( const vec3& eye, const vec3& center, const vec3& up );
};

std::ostream &operator<<( std::ostream& os, const Camera& camera );


