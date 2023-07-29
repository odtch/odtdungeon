#pragma once

#include "PhysicsCore.h"
#include "utils/Module.h"

class Physics : public Module
{
private:
	static Physics* _Global;
public:
	static const char* Id;
//	static Physics* Get(){ return Singleton::Get<Physics>(); }
	static Physics* Get();
private:
	JPH::TempAllocatorImpl* _temp_allocator = null;
	JPH::JobSystemThreadPool* _job_system = null;
	MyBPLayerInterfaceImpl* _broad_phase_layer_interface = null;
	MyBodyActivationListener* _body_activation_listener = null;
	MyContactListener* _contact_listener = null;
private: public:
	JPH::PhysicsSystem* _physics_system = null;
	JPH::BodyInterface* _body_interface = null;
	uint _step = 0;
private:
	List<PhysicsBody*> _bodies;
	List<PhysicsCharacter*> _characters;
private:
	PerformanceCounter* _jolt_pc;
	PerformanceCounter* _outside_pc;
private:
public:
	explicit Physics();
	virtual ~Physics() override;
public:
	virtual void start() override;
	virtual void stop() override;
public:
	void update( float dt );
public:
//	PhysicsShape* createShapeSphere( float radius );
public:
//	void createBody( Object* object, const ::Vec3& pos );
public:
//	virtual PhysicsShape* createSphereShape( float radius );
//	virtual PhysicsShape* createBoxShape( const Vec3& radius );
//	virtual PhysicsShape* createConvexHullShape( const MeshPNT& mesh );
//	virtual PhysicsShape* createConvexHullShape( const MeshPNT& mesh, std::function<bool( const VertexPNT& vertex )> filter );
//	virtual PhysicsShape* createMeshShape( const MeshPNT& mesh );
public:
//	virtual PhysicsDynamicBody* createDynamicBody( Object* object, PhysicsShape* shape );
//	virtual PhysicsStaticBody* createStaticBody( Object* object, PhysicsShape* shape );
//private:
//	void onDynamicBodyDestroyed( PhysicsDynamicBody* body );
//	void onCharacterCreated( PhysicsCharacter* character );
//	void onCharacterDestroyed( PhysicsCharacter* character );
public:
	friend class PhysicsBody;
//	friend class PhysicsCharacter;
	friend class PhysicsDebugRenderer;
};
