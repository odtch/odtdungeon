#include "stdodt.h"

#include "RenderLight.h"


RenderLight* RenderLight::CreateAmbient(const vec4 &color){
	RenderLight* light = new RenderLight();
	light->_type = Ambient;
	light->_color = color;
	return light;
}
RenderLight* RenderLight::CreateDirectional( const Vec3 &direction, const glm::vec4 &color ){
	RenderLight* light = new RenderLight();
	light->_type = Directional;
	light->_direction = direction;
	light->_color = color;
	return light;
}
RenderLight* RenderLight::CreatePoint( const Vec3 &position, float range, const glm::vec4 &color ){
	RenderLight* light = new RenderLight();	
	light->_type = Point;
	light->_position = position;
	light->_range = range;
	light->_color = color;
	return light;
}
RenderLight* RenderLight::CreateSpot(const Vec3 &position, const Vec3 &direction, float angle, float range, const glm::vec4 &color){
	RenderLight* light = new RenderLight();
	light->_type = Directional;
	light->_direction = direction;
	light->_angle = angle;
	light->_range = range;
	light->_color = color;
	return light;
}

RenderLight::RenderLight(){
}
RenderLight::~RenderLight(){
}
void RenderLight::setPosition( const Vec3& position ){
	_position = position;
	_update_counter++;
}
void RenderLight::setDirection(const Vec3& direction){
	_direction = direction;
	_update_counter++;
}
