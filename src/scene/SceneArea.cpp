#include "stdodt.h"

#include "SceneArea.h"
#include "renderer/RenderLayer.h"

SceneArea::SceneArea( const String& name, RenderLayer* layer )
	:_name( name )
	,_layer( asserted( layer ) )
{
	//_layer = new RenderLayer();
}
SceneArea::~SceneArea(){
	deleteAllChilds();
//	delete _layer;
//	_layer = null;
}
//void SceneArea::setTransformInParent( const Mat4& transform, float scale ){
//	_transformToParent = transform * Mat4::Scale( 1.0f / scale );
//	_transformFromParent = Mat4::Scale( scale ) * transform.inverted();
//}
//PosOri SceneArea::transformPosOriFromParentToLocal( const PosOri& posoriInParent ){
//	PosOri posoriInChild;
//	posoriInChild.set( _transformFromParent * posoriInParent.matrix() );
//	return posoriInChild;
//}
//PosOri SceneArea::transformPosOriFromLocalToParent(const PosOri& posoriInLocal){
//	PosOri posoriInParent;
//	posoriInParent.set( _transformToParent * posoriInLocal.matrix() );
//	return posoriInParent;
//}
//void SceneArea::addRenderable( Renderable* renderable ){
//	_layer->addRenderable( renderable );
//}
//void SceneArea::addRenderables( const List<Renderable*>& renderables ){
//	for( auto renderable : renderables ){
//		addRenderable( renderable );
//	}
//}
//void SceneArea::removeRenderable( Renderable* renderable ){
//	_layer->removeRenderable( renderable );
//}
//void SceneArea::removeRenderables( const List<Renderable*>& renderables ){
//	for( auto renderable : renderables ){
//		removeRenderable( renderable );
//	}
//}

////PropertyType SceneAreaShifter::Type( "SceneAreaShifter" );
////SceneAreaShifter::SceneAreaShifter(const String& name, float radius, SceneArea* target_area )
////	:Property( &Type )
////	,_name( name )
////	,_radius( radius )
////	,_target_area( target_area )
////{
////	assert( _target_area );
////}
////SceneAreaShifter::~SceneAreaShifter(){
////}
////void SceneAreaShifter::bind( PropertyObject* object ){
////	Property::bind( object );
////	_posori = object->getProperty<PosOriProperty>();
////}

////PropertyType SceneAreaJumper::Type( "SceneAreaJumper" );
////SceneAreaJumper::SceneAreaJumper( float radius )
////	:AnimatedProperty( &Type )
////	,_radius( radius )
////{
////}
////SceneAreaJumper::~SceneAreaJumper(){
////}
////void SceneAreaJumper::bind( PropertyObject* object ){
////	assert( _object == null );
////	_object = dynamic_cast<SceneObject*>( object );
////	assert( _object );
////	AnimatedProperty::bind( object );
////	_posori = object->getProperty<PosOriProperty>();
////	_object->addListener( this );

////}
////void SceneAreaJumper::animate( float dt ){
////	SceneArea* area = _object->area();
////	if( _prev_area != area ){
////		logDebug( "Jumper is now in new area", area );
////		_prev_area = area;
////	}
////	if( area == null )
////		return;
////	_next_shifter = null;
////	for( SceneNode* node : area->childs() ){
////		checkForShifter( node );
////	}
////	if( _next_shifter ){
////		logDebug( "jumper founds shifter", _next_shifter->name() );
////		SceneArea* target_area = _next_shifter->target_area();
////		if( area == target_area ){
////			logError( "jumper founds shifter to existing area" );
////		} else {
////			logDebug( "jumper remove" );
////			_object->parent()->removeChild( _object );
////			logDebug( "jumper add" );
////			target_area->addChild( _object );
////		}
////	}
////}
////void SceneAreaJumper::checkForShifter( SceneNode* node ){
////	SceneAreaShifter* shifter = node->findProperty<SceneAreaShifter>();
////	if( shifter ){
////		Vec3 offset = ( shifter->position() - position() );
////		float len2 = offset.length2();
////		float minlen = _radius + shifter->radius();
////		float minlen2 = minlen * minlen;
////		if( len2 <= minlen2 ){
////			if( _next_shifter ){
////				logError( "SceneAreaJumper multiple Shifters found" );
////			}
////			_next_shifter = shifter;
////		}
////	}
////	for( SceneNode* child : node->childs() ){
////		checkForShifter( child );
////	}
////}
