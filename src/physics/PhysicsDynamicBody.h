//#pragma once

//#include "PhysicsCore.h"
//#include "JoltCore.h"

//class PhysicsDynamicBodyType : public ObjectPropertyType
//{
//private:
//	PhysicsShape* _shape;
//public:
//	explicit PhysicsDynamicBodyType( ObjectType* object, PhysicsShape* shape );
//	virtual ~PhysicsDynamicBodyType();
//public:
//	PhysicsShape* shape() const{ return _shape; }
//public:
//	virtual void newInstance( Object* object ) override;
//};

//class PhysicsDynamicBody : public ObjectProperty
//{
//private:
//	JoltPhysics* _jolt = null;
//	JPH::BodyID* _body_id = null;
//public:
//	explicit PhysicsDynamicBody( Object* object, PhysicsDynamicBodyType* type ); //const JPH::BodyID& body_id, PosOriProperty* posori, JoltPhysics* physics );
//	virtual ~PhysicsDynamicBody();
//public:
//	PhysicsDynamicBodyType* type() const{ return (PhysicsDynamicBodyType*) ObjectProperty::type(); }
//public:
//	virtual void onAddedToScene( Scene* scene ) override;
//	virtual void onRemovedFromScene( Scene* scene ) override;
//public:
//	void onPhysicsUpdated();
//public:
//	friend class JoltPhysics;
//};

