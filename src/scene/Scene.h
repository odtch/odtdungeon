#pragma once

#include "utils/Threading.h"
#include "SceneCore.h"
#include "SceneNode.h"

class Scene : public Thread, private SceneNode
{
	DEBUGCOUNTER(Scene)
//private:
//	SceneRootNode* _root = null;
private:
//	SceneArea* _root_space;
//public:
//	bool pause_animate = false;
private:
	float _rest_dt = 0;
    //List<SceneListener*> _listeners;
public:
	explicit Scene();
    virtual ~Scene() override;
public:
	const List<SceneNode*>& childs() const{ return SceneNode::childs(); }
	void add( SceneNode* node );
	void remove( SceneNode* node );
	void clear();	
public:
//	SceneNode* root() const;
//	SceneArea* root_space() const{ return _root_space; }
public:
//	const List<SceneListener*>& listeners() const{ return _listeners; }
//	void addListener( SceneListener* listener );
//	void removeListener( SceneListener* listener );
public:
	virtual Scene* findScene() override{ return this; }	
public:
	virtual void animate( float dt ) override;
protected:
    virtual void run() override;
};
