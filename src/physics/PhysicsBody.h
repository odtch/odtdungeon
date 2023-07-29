#pragma once

#include "PhysicsCore.h"

enum PhysicsMotionType {
	PhysicsMotionType_Invalid,
	PhysicsMotionType_Dynamic,
	PhysicsMotionType_Static,
	PhysicsMotionType_Kinematic
};

class PhysicsBodyType //: public ObjectPosOriPropertyType
{
private:
	PhysicsShape* _shape = null;
	PhysicsMotionType _motiontype = PhysicsMotionType_Invalid;
public:
	explicit PhysicsBodyType( /*ObjectType* object, */PhysicsShape* shape, PhysicsMotionType motiontype = PhysicsMotionType_Dynamic );
	virtual ~PhysicsBodyType();
public:
	PhysicsShape* shape() const{ return _shape; }
	PhysicsMotionType motiontype() const{ return _motiontype; }
public:
	//virtual ObjectProperty* newInstance( Object* object, ObjectPosOriProperty* parent ) override;
};

class PhysicsBody : public SceneProperty
{
private:
	SceneObject* _object;
private:
	Physics* _physics = null;
private:
	PhysicsShape* _shape = null;
	PhysicsMotionType _motiontype = PhysicsMotionType_Invalid;
private: public:
	JPH::Body* _jph = null;
	//JPH::BodyID* _body_id = null;
public:
	PhysicsBody* _ignoreCollisionsWith = null;
public:
//	explicit PhysicsBody( PhysicsBodyType* type, ObjectPosOriProperty* parent, Object* object );
	explicit PhysicsBody( PhysicsShape* shape, PhysicsMotionType motiontype, SceneObject* object );
//	explicit PhysicsBody( PhysicsShape* shape, PhysicsMotionType motiontype, ObjectPosOriProperty* parent, Object* object );
	virtual ~PhysicsBody() override;
public:
//	PhysicsBodyType* type() const{ return (PhysicsBodyType*) ObjectProperty::type(); }
public:
	PhysicsMotionType motiontype() const{ return _motiontype; }
	void setMotionType( PhysicsMotionType motiontype );
public:
	Physics* physics(){ ASSERT( _physics ); return _physics; }
	JPH::Body* body(){ ASSERT( _jph ); return _jph; }
	//const JPH::BodyID& body_id() const{ ASSERT( _body_id ); return *_body_id; }
public:
	void moveKinematic( const PosOri& posori );
public:
	virtual void onAddedToScene( Scene* scene ) override;
	virtual void onRemovedFromScene( Scene* scene ) override;
public:
	virtual void animate( float dt ) override;
public:
	void onPhysicsUpdated();
public:
	friend class Physics;
};

