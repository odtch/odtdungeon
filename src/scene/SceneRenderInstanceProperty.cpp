#include "stdodt.h"

#include "SceneRenderInstanceProperty.h"
#include "Scene.h"
#include "SceneArea.h"

SceneRenderInstanceProperty::SceneRenderInstanceProperty( SceneObject* object )
	:SceneProperty( object )
	,_object( asserted( object ) )
{
}
SceneRenderInstanceProperty::~SceneRenderInstanceProperty(){
	assert( _instance == null );
}
void SceneRenderInstanceProperty::onAddedToScene( Scene* scene ){
	assert( _instance == null );
	_instance = createInstance( scene->renderer(), _object->area()->layer() );
	recalcPosOri();
}
void SceneRenderInstanceProperty::onRemovedFromScene( Scene* scene ){
	if( _instance ){
		int todo;
		//odelete( _instance );
		_instance = null;
	}
}
void SceneRenderInstanceProperty::recalcPosOri(){
	_posori = _object->posori();
}
void SceneRenderInstanceProperty::animate( float dt ){
	recalcPosOri();
	if( _instance == null ){
		logWarning( "SceneRenderInstanceProperty::animate no instance" );
	} else {
		_instance->setPosOri( _posori );
	}
}

SceneRenderInstancePNTProperty::SceneRenderInstancePNTProperty( Mesh<VertexPNT>* mesh, Material* material, SceneObject* object )
	:SceneRenderInstanceProperty( object )
	,_mesh( asserted( mesh ) )
	,_material( asserted( material ) )
{
}
SceneRenderInstancePNTProperty::~SceneRenderInstancePNTProperty(){
}
RenderInstance* SceneRenderInstancePNTProperty::createInstance( Renderer& renderer, RenderLayer* layer ){
	return renderer.createInstance( layer, posori(), _mesh, _material );
}
