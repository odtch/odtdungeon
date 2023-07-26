#pragma once

#include "SceneCore.h"

class SceneProperty
{
	DEBUGCOUNTER( SceneProperty )
	ODTNOCOPY( SceneProperty )
private:
	SceneNode* _node;
	SceneProperty* _next = null;
public:
	explicit SceneProperty( SceneNode* node );
	virtual ~SceneProperty();
public:
	SceneNode* node() const{ return _node; }
protected:
	virtual void onAddedToScene( Scene* scene ){ OUNUSED( scene ); }
	virtual void onRemovedFromScene( Scene* scene ){ OUNUSED( scene ); }
protected:
	virtual void animate( float dt ){ OUNUSED( dt ); }
public:
	friend class SceneNode;
};
