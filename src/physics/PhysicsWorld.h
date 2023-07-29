#pragma once

#include "PhysicsCore.h"

class PhysicsWorld : public SceneProperty
{
private:
	Scene* _scene;
private:
	Physics* _physics = null;
public:
	explicit PhysicsWorld( Physics* physics, Scene* scene );
	virtual ~PhysicsWorld() override;
public:
	virtual void animate( float dt ) override;
};

