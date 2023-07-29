#include "stdodt.h"
#include "PhysicsWorld.h"
#include "Physics.h"
#include "JoltImpl.h"

PhysicsWorld::PhysicsWorld(Physics* physics, Scene* scene)
	:SceneProperty( scene )
	,_scene( asserted( scene ) )
	,_physics( asserted( physics ) )
{
}
PhysicsWorld::~PhysicsWorld(){
}
void PhysicsWorld::animate( float dt ){
	_physics->update( dt );

}
