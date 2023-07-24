#pragma once

#include "SceneCore.h"

class SceneNode
{
    //: public
    //: PropertyObject public NamedObject, public EventReceiver
	DEBUGCOUNTER( SceneNode )
	ODTNOCOPY( SceneNode )
//public:
//	static EventType AddedToScene;
//	static EventType RemovedFromScene;
//	static EventType Animate;
//protected:
//	SceneNodeType* _type = null;
private:
	SceneNode* _parent = null;
	List<SceneNode*> _childs;
private:
//	bool _atAnimate = false;
//	List<SceneNode*> _childsToAdd;
//	List<SceneNode*> _childsToRemove;
public:
	explicit SceneNode();
	virtual ~SceneNode();
public:
	Scene* scene();
	bool isInScene();
	virtual Scene* findScene();
//public:
//	virtual bool isInArea() const;
//	virtual SceneArea* area();
public:
	bool hasParent() const{ return _parent != null; }
	SceneNode* parent() const{ assert( _parent ); return _parent; }
	const SceneNode* root() const;
public:
	const List<SceneNode*>& childs() const{ return _childs; }
	void addChild( SceneNode* child );
	void removeChild( SceneNode* child );
//	void moveChildToLastAnimated( SceneNode* child );
	void deleteAllChilds();
//public:
//	void moveToParent( SceneNode* parent );
protected:
	virtual void onAddedToScene( Scene* scene );
	virtual void onRemovedFromScene( Scene* scene );
//protected:
//	virtual void onWorldTranslation( const Vec3& translation );
//public:
//	void iterate( std::function< void( SceneNode* node )> handler );
//public:
//	template<class T> T* findNearest( const Vec3& source_position, std::function<bool(T*)> filter ){
//		T* best_node = null;
//		float best_dist2 = 0;
//		iterate( [&best_node, &best_dist2, &source_position, &filter ]( SceneNode* node ){
//			T* this_node = dynamic_cast<T*>( node );
//			if( this_node ){
//				if( filter( this_node ) ){
//					float this_dist = ( this_node->position() - source_position ).length2();
//					if( best_node == null || this_dist < best_dist2 ){
//						best_node = this_node;
//						best_dist2 = this_dist;
//					}
//				}
//			}
//		} );
//		return best_node;
//	}
public:
	virtual void animate( float dt );
//public:
//	virtual void render( Renderer& renderer );
//public:
//	virtual bool isRootNode() const { return false; }
//public:
//	virtual bool isObject() const{ return false; }
//	virtual Object* asObject(){ assert( false ); }
//	virtual bool isArea() const{ return false; }
//	virtual SceneArea* asArea(){ assert( false ); }
//	virtual bool isAreaObject() const{ return false; }
//	virtual SceneAreaObject* asAreaObject(){ assert( false ); }
//public:
//	virtual bool isPhysicsObject() const { return false; }
//	virtual PhysicsObject* asPhysicsObject(){ assert( false ); }
//public:
//	friend class Scene;
};

//class SceneResourceHolderNode : public SceneNode
//{
//private:
//	Resource* _resource;
//public:
//	explicit SceneResourceHolderNode( Resource* resource );
//	virtual ~SceneResourceHolderNode() override;
//public:
//};

////class SceneRenderableNode : public SceneNode
////{
////private:
////	Renderable* _renderable;
////public:
////	explicit SceneRenderableNode( Renderable* renderable );
////	virtual ~SceneRenderableNode() override;
////public:
////	void setRenderable( Renderable* renderable );
////public:
////	virtual void render( Renderer& renderer ) override;
////};


//class SceneRootNode : public SceneNode
//{
//private:
//	Scene* _scene;
//public:
//	explicit SceneRootNode( Scene* scene );
//	virtual ~SceneRootNode() override;
//public:
//	Scene* scene() const{ return _scene; }
//public:
//	virtual bool isRootNode() const override { return true; }
//};
