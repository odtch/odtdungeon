#include "stdodt.h"

#include "Scene.h"
//#include "SceneListener.h"
#include "SceneNode.h"

Scene::Scene(){
//	_root = new SceneRootNode( this );
}
Scene::~Scene(){
//	ASSERT( _listeners.isEmpty() );
	ASSERT( childs().isEmpty() );
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
        logDebug( "Scene::run" );
        sleep_ms( 1000 );
    }
}
//void Scene::render( Renderer& renderer ){
//	_root->render( renderer );
//}


