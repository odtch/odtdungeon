#include "stdodt.h"

#include "SceneObject.h"

SceneObject::SceneObject(){
}
SceneObject::~SceneObject(){
}
void SceneObject::setPosOri( const PosOri& posori ){
	_posori = posori;
}
