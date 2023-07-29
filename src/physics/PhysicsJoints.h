#pragma once

#include "PhysicsCore.h"

//class PhysicsJointType : public ObjectConnectionType
//{
//private:
//public:
//	explicit PhysicsJointType( ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector );
//	virtual ~PhysicsJointType();
//public:
//	virtual ObjectConnection* newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ) override;
//};

//class PhysicsJoint : public ObjectConnection
//{
//private:
//	Vec3 _rotation;
//public:
//	explicit PhysicsJoint( ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsJointType* type );
//	virtual ~PhysicsJoint();
//public:
//	PhysicsJointType* type() const{ return (PhysicsJointType*) ObjectConnection::type(); }
//public:
//	virtual void onParentPosOriChanged() override;
//public:
//	virtual void animate( float dt ) override;
//private:
//	void reposChild();
//};



//class PhysicsFixedJointType : public PhysicsJointType
//{
//private:
//public:
//	explicit PhysicsFixedJointType( ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector );
//	virtual ~PhysicsFixedJointType();
//public:
//	virtual PhysicsJoint* newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ) override;
//};

//class PhysicsFixedJoint : public PhysicsJoint
//{
//public:
//	explicit PhysicsFixedJoint( ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsFixedJointType* type );
//	virtual ~PhysicsFixedJoint();
//public:
//	PhysicsFixedJointType* type() const{ return (PhysicsFixedJointType*) ObjectConnection::type(); }
//public:
//	virtual void onAddedToScene( Scene* scene ) override;
//	virtual void onRemovedFromScene( Scene* scene ) override;
//public:
//	virtual void onParentPosOriChanged() override;
//public:
//	virtual void animate( float dt ) override;
//private:
//	void reposChild();
//};




//class PhysicsHingeJointType : public PhysicsJointType
//{
//private:
//public:
//	Vec3 _parentAxis = Vec3::Up;
//	Vec3 _parentNormal = Vec3::Forward;
//	Vec3 _childAxis = Vec3::Up;
//	Vec3 _childNormal = Vec3::Forward;
//public:
//	explicit PhysicsHingeJointType( ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector );
//	virtual ~PhysicsHingeJointType();
//public:
//	virtual PhysicsJoint* newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ) override;
//};

//class PhysicsHingeJoint : public PhysicsJoint
//{
//private:
//	Vec3 _rotation;
//					public:
//	JPH::HingeConstraint* _jph = null;
//public:
//	explicit PhysicsHingeJoint( ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsHingeJointType* type );
//	virtual ~PhysicsHingeJoint();
//public:
//	PhysicsHingeJointType* type() const{ return (PhysicsHingeJointType*) ObjectConnection::type(); }
//public:
//	virtual void onAddedToScene( Scene* scene ) override;
//	virtual void onRemovedFromScene( Scene* scene ) override;
//public:
//	virtual void onParentPosOriChanged() override;
//public:
//	virtual void animate( float dt ) override;
//private:
//	void reposChild();
//};

