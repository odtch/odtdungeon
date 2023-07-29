#pragma once

//#include "PhysicsCore.h"
//#include "PhysicsBody.h"
//#include "JoltCore.h"

//class PhysicsStaticBodyType : public ObjectPropertyType
//{
//private:
//	PhysicsShape* _shape;
//public:
//	explicit PhysicsStaticBodyType( ObjectType* object, PhysicsShape* shape );
//	virtual ~PhysicsStaticBodyType() override;
//public:
//	PhysicsShape* shape() const{ return _shape; }
//public:
//	virtual void newInstance( Object* object ) override;
//};

//class PhysicsStaticBody : public ObjectProperty
//{
//private:
//	JoltPhysics* _jolt = null;
//	JPH::BodyID* _body_id = null;
//public:
//	explicit PhysicsStaticBody( Object* object, PhysicsStaticBodyType* type );
//	virtual ~PhysicsStaticBody();
//public:
//	PhysicsStaticBodyType* type() const{ return (PhysicsStaticBodyType*) ObjectProperty::type(); }
//public:
//	virtual void onAddedToScene( Scene* scene ) override;
//	virtual void onRemovedFromScene( Scene* scene ) override;
//};

