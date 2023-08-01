#pragma once

#include "utils/Debugging.h"
#include "math/Vec3.h"

class RenderLight
{
	DEBUGCOUNTER( RenderLight )
public:
	static RenderLight* CreateAmbient( const vec4& color );
	static RenderLight* CreateDirectional( const Vec3& direction, const vec4& color );
	static RenderLight* CreatePoint( const Vec3& position, float range, const vec4& color );
	static RenderLight* CreateSpot( const Vec3& position, const Vec3& direction, float angle, float range, const vec4& color );
public:
	enum Type {
		Ambient, Directional, Point, Spot
	};
private:
	Type _type;
private: public:
	Vec3 _position;
	Vec3 _direction;
	float _range = 0;
	float _angle = 0;
	vec4 _color;
private:
	int _update_counter = 1;
public:
	mutable int _last_vulkan_index = -1;
	mutable int _last_vulkan_uc = -1;
private:
	explicit RenderLight();
public:
	virtual ~RenderLight();
public:
	Type type() const{ return _type; }
	const Vec3& position() const{ return _position; }
	void setPosition( const Vec3& position );
	const Vec3& direction() const{ return _direction; }
	void setDirection( const Vec3& direction );
	float range() const{ return _range; }
	float angle() const{ return _angle; }
	const vec4& color() const{ return _color; }
public:
	int updateCounter() const{ return _update_counter; }
};
