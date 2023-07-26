#include "stdodt.h"

#include "Scene.h"
//#include "SceneListener.h"
#include "SceneNode.h"

Scene::Scene( Renderer* renderer )
    :_renderer( asserted( renderer ) )
	,_frameratelimiter( FPS )
{
//	_root = new SceneRootNode( this );
}
Scene::~Scene(){
//	ASSERT( _listeners.isEmpty() );
	ASSERT( childs().isEmpty() );
    _renderer = null;
}
void Scene::add( SceneNode* node ){
	addChild( node );
}
void Scene::remove( SceneNode* node ){
	removeChild( node );
}
void Scene::clear(){
	deleteAllChilds();
}
//void Scene::addListener( SceneListener* listener ){
//	assert( listener );
//	_listeners.add( listener );
//}
//void Scene::removeListener( SceneListener* listener ){
//	_listeners.remove( listener );
//}
//SceneNode* Scene::root() const{
//	return _root;
//}
void Scene::animate( float dt ){	
//	_rest_dt += dt;
//	dt = _rest_dt;
//	static const float max_dt = 1.0f / 60.0f;
//	if( dt > max_dt )
//		dt = max_dt;
//	_rest_dt -= dt;
    SceneNode::animate( dt );
    //	_root->animate( dt );
}
void Scene::run(){
    while( !should_stop() ){
		//logDebug( "Scene::run" );
		//_frameratelimiter._debug = true;
		_frameratelimiter.tick();
		sleep_ms( 1 );
		animate( 1.0f / FPS );
		if( _fps.tick() ){
			logDebug( "Scene::run", _fps.fps(), _frameratelimiter._sleep_time_per_frame.toString() );
			_frameratelimiter._sleep_time_per_frame.reset();
		}
    }
}
//void Scene::render( Renderer& renderer ){
//	_root->render( renderer );
//}


