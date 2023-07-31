#include "stdodt.h"

#include "SceneNode.h"
#include "Scene.h"
//#include "SceneListener.h"

//EventType SceneNode::AddedToScene( "AddedToScene" );
//EventType SceneNode::RemovedFromScene( "RemovedToScene" );

SceneNode::SceneNode(){
}
SceneNode::~SceneNode(){
	ASSERT( _parent == null );
	ASSERT( _childs.isEmpty() );
	SceneProperty* property = _firstProperty;
	while( property ){
		SceneProperty* p = property;
		property = property->_next;
		delete p;
	}
}
Scene* SceneNode::scene(){
	Scene* scene = findScene();
	assert( scene );
	return scene;
}
bool SceneNode::isInScene(){
	return findScene() != null;
}
Scene* SceneNode::findScene(){
	if( _parent )
		return _parent->findScene();
	return null;
}
bool SceneNode::isInArea(){
	return findArea() != null;
}
SceneArea* SceneNode::area(){
	SceneArea* area = findArea();
	if( area == null ){
		logError( "SceneNode::area not in area" );
		assert( false );
	}
	return area;
}
SceneArea* SceneNode::findArea(){
	if( _parent )
		return _parent->findArea();
	return null;
}
//bool SceneNode::isInArea() const{
//	if( _parent )
//		return _parent->isInArea();
//	return false;
//}
//SceneArea* SceneNode::area(){
//	if( _parent )
//		return _parent->area();
//	assert( false );
//}
const SceneNode* SceneNode::root() const {
	const SceneNode* node = this;
	while( node->_parent ){
		node = node->_parent;
	}
	return node;
}
void SceneNode::addChild( SceneNode* child ){
	//ASSERT( !_atAnimate );
	assert( child );
	assert( child->_parent == null );
	child->_parent = this;
	_childs.add( child );
	Scene* scene = findScene();
	if( scene ){
		child->onAddedToScene( scene );
	}
}
void SceneNode::removeChild( SceneNode* child ){
	assert( child );
//	if( _atAnimate ){
//		_childsToRemove.add( child );
//		return;
//	}
	assert( child->_parent == this );
	child->_parent = null;
	_childs.remove( child );
	Scene* scene = findScene();
	if( scene ){
		child->onRemovedFromScene( scene );
	}
}
//void SceneNode::moveChildToLastAnimated( SceneNode* child ){
//	_childs.remove( child );
//	_childs.add( child );
//}
void SceneNode::deleteAllChilds(){
	while( !_childs.isEmpty() ){
		auto child = _childs.last();
		removeChild( child );
		delete child;
	}
}
void SceneNode::registerProperty( SceneProperty* property ){
	assert( property );
	ASSERT( property->_next == null );
	if( _firstProperty == null ){
		_firstProperty = property;
	} else {
		SceneProperty* prev = _firstProperty;
		while( prev->_next ){
			prev = prev->_next;
		}
		ASSERT( prev->_next == null );
		prev->_next = property;
	}
}
//void SceneNode::moveToParent( SceneNode* parent ){
//	assert( parent );
//	assert( _parent );
//	if( _parent == parent )
//		return;
//	_parent->_childs.remove( this );
//	_parent = parent;
//	_parent->_childs.add( this );
//}
void SceneNode::onAddedToScene( Scene* scene ){
//	for( auto listener : scene->listeners() )
//		listener->onAddedToScene( this );
	SceneProperty* property = _firstProperty;
	while( property ){
		property->onAddedToScene( scene );
		property = property->_next;
	}
	for( SceneNode* child : _childs ){
		child->onAddedToScene( scene );
	}
//	send( &AddedToScene );
}
void SceneNode::onRemovedFromScene( Scene* scene ){
	for( SceneNode* child : _childs ){
		child->onRemovedFromScene( scene );
	}
	SceneProperty* property = _firstProperty;
	while( property ){
		property->onRemovedFromScene( scene );
		property = property->_next;
	}
//	for( auto listener : scene->listeners() )
//		listener->onRemovedFromScene( this );
//	send( &RemovedFromScene );
}
//void SceneNode::onWorldTranslation(const Vec3& translation)
//{
//	for( SceneNode* child : _childs ){
//		child->onWorldTranslation( translation );
//	}
//}
//void SceneNode::iterate( std::function<void (SceneNode*)> handler ){
//	handler( this );
//	for( SceneNode* child : childs() ){
//		child->iterate( handler );
//	}
//}
void SceneNode::animate( float dt ){
	assert( 0 < dt );
	//ASSERT( !_atAnimate );
	//_atAnimate = true;
	SceneProperty* property = _firstProperty;
	while( property ){
		property->animate( dt );
		property = property->_next;
	}
	int prev_size = _childs.size();
	for( SceneNode* child : _childs ){
		ASSERT( child->_parent == this );
		child->animate( dt );
		if( child->_parent != this ){
			logDebug( "SceneNode::animate removed child unimplemented" );
			return;
		}
		//ASSERT( prev_size == _childs.size() );
	}
	//ASSERT( _atAnimate );
	//_atAnimate = false;
	//while( !_childsToRemove( ))
//	for( int c = 0; c < _childs.size(); c++ ){
//		SceneNode* child = _childs.get( c );
//		child->animate( dt );
//		if( c < _childs.size() ){
//			if( _childs.get( c ) != child ){
//				c--;
//			}
//		}
//	}
//	send( &Animate );
}
//void SceneNode::render( Renderer& renderer ){
//	if( !_childs.isEmpty() ){
//		//Mat4 prev = renderer.addTransform( posori() );
//		for( SceneNode* child : _childs ){
//			child->render( renderer );
//		}
//		//renderer.setTransform( prev );
//	}
//}


//SceneResourceHolderNode::SceneResourceHolderNode(Resource* resource)
//	:_resource( resource )
//{
//	assert( _resource );

//}
//SceneResourceHolderNode::~SceneResourceHolderNode(){
//	delete _resource;
//}

////SceneRenderableNode::SceneRenderableNode(Renderable* renderable)
////	:_renderable( renderable )
////{
////}
////SceneRenderableNode::~SceneRenderableNode(){
////}
////void SceneRenderableNode::setRenderable( Renderable* renderable ){
////	_renderable = renderable;
////}
////void SceneRenderableNode::render( Renderer& renderer ){
////	if( _renderable )
////		renderer.add( posori().matrix(), _renderable );
////}

//SceneRootNode::SceneRootNode( Scene* scene )
//	:_scene( scene )
//{
//	assert( _scene );
//}
//SceneRootNode::~SceneRootNode(){
//}
