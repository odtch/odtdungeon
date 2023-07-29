#include "stdodt.h"
#include "PhysicsJoints.h"
#include "PhysicsBody.h"
#include "JoltImpl.h"

//PhysicsJointType::PhysicsJointType(ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector)
//	:ObjectConnectionType( parentConnector, childConnector )
//{
//}
//PhysicsJointType::~PhysicsJointType(){
//}
//ObjectConnection* PhysicsJointType::newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ){
//	return new PhysicsJoint( parentConnector, childConnector, this );
//}

//PhysicsJoint::PhysicsJoint(ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsJointType* type)
//	:ObjectConnection( parentConnector, childConnector, type )
//{
//}
//PhysicsJoint::~PhysicsJoint(){
//}
//void PhysicsJoint::onParentPosOriChanged(){
//	reposChild();
//}
//void PhysicsJoint::animate( float dt ){
//	OUNUSED( dt );
//	_rotation += Vec3( 0, 0, dt * 15 );

//}
//void PhysicsJoint::reposChild(){
//	PosOri absPosOri( parentObject()->posori().matrix() * parentConnector()->type()->posori().matrix() );
//	PosOri childPosOri( absPosOri.matrix() * childConnector()->type()->posori().matrix().inverted() );
//	childPosOri.rotate( _rotation.z(), childPosOri.up() );
//	childObject()->setPosOri( childPosOri );
//}





//PhysicsFixedJointType::PhysicsFixedJointType(ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector)
//	:PhysicsJointType( parentConnector, childConnector )
//{
//}
//PhysicsFixedJointType::~PhysicsFixedJointType(){
//}
//PhysicsJoint* PhysicsFixedJointType::newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ){
//	return new PhysicsFixedJoint( parentConnector, childConnector, this );
//}

//PhysicsFixedJoint::PhysicsFixedJoint(ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsFixedJointType* type)
//	:PhysicsJoint( parentConnector, childConnector, type )
//{
//}
//PhysicsFixedJoint::~PhysicsFixedJoint(){
//}
//void PhysicsFixedJoint::onAddedToScene(Scene* scene){
//	JPH::FixedConstraintSettings settings;
//	settings.mEnabled = true;
//	//settings.mMotorSettings.
//	settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM; // WorldSpace;
//	settings.mPoint1 = parentConnector()->type()->posori().position().toJPH();
//	settings.mPoint2 = childConnector()->type()->posori().position().toJPH();
//	PhysicsBody* parentBody = parentObject()->getProperty<PhysicsBody>();
//	PhysicsBody* childBody = childObject()->getProperty<PhysicsBody>();
//	assert( childBody->_ignoreCollisionsWith == null );
//	childBody->_ignoreCollisionsWith = parentBody;
//	JPH::Body* parentJolt = parentBody->body(); // JoltPhysics::Get()->_physics_system->GetBodyLockInterface().TryGetBody( parentBody->body_id() );
//	assert( parentJolt );
//	JPH::Body* childJolt = childBody->body(); //JoltPhysics::Get()->_physics_system->GetBodyLockInterface().TryGetBody( childBody->body_id() );
//	assert( childJolt );
//	JPH::TwoBodyConstraint* constraint = settings.Create( *parentJolt, *childJolt );
//	ASSERT( constraint );
//	Physics::Get()->_physics_system->AddConstraint( constraint );
//}
//void PhysicsFixedJoint::onRemovedFromScene(Scene* scene){
//}
//void PhysicsFixedJoint::onParentPosOriChanged(){
//	if( childObject()->isInScene() == false ){
//		reposChild();
//	}
//}
//void PhysicsFixedJoint::animate( float dt ){
//	OUNUSED( dt );
//}
//void PhysicsFixedJoint::reposChild(){
////	PosOri absPosOri( parentObject()->posori().matrix() * parentConnector()->type()->posori().matrix() );
////	PosOri childPosOri( absPosOri.matrix() * childConnector()->type()->posori().matrix().inverted() );
////	childPosOri.rotate( _rotation.z(), childPosOri.up() );
////	childObject()->setPosOri( childPosOri );
//}


//PhysicsHingeJointType::PhysicsHingeJointType(ObjectConnectorType* parentConnector, ObjectConnectorType* childConnector)
//	:PhysicsJointType( parentConnector, childConnector )
//{
//}
//PhysicsHingeJointType::~PhysicsHingeJointType(){
//}
//PhysicsJoint* PhysicsHingeJointType::newInstance( ObjectConnector* parentConnector, ObjectConnector* childConnector ){
//	return new PhysicsHingeJoint( parentConnector, childConnector, this );
//}

//PhysicsHingeJoint::PhysicsHingeJoint(ObjectConnector* parentConnector, ObjectConnector* childConnector, PhysicsHingeJointType* type)
//	:PhysicsJoint( parentConnector, childConnector, type )
//{
//}
//PhysicsHingeJoint::~PhysicsHingeJoint(){
//}
//void PhysicsHingeJoint::onAddedToScene( Scene* scene ){
//	PhysicsHingeJointType* type = this->type();
//	JPH::HingeConstraintSettings settings;
//	settings.mEnabled = true;
//	//settings.mMotorSettings.
//	settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM; // WorldSpace;
//	settings.mPoint1 = parentConnector()->type()->posori().position().toJPH();
//	settings.mPoint2 = childConnector()->type()->posori().position().toJPH();
////	settings.mHingeAxis1 = settings.mHingeAxis2 = JPH::Vec3::sAxisZ();
////	settings.mNormalAxis1 = settings.mNormalAxis2 = JPH::Vec3::sAxisY();

//	settings.mHingeAxis1 = type->_parentAxis.toJPH();
//	settings.mHingeAxis2 = type->_childAxis.toJPH();
//	settings.mNormalAxis1 = type->_parentNormal.toJPH();
//	settings.mNormalAxis2 = type->_childNormal.toJPH();

////	settings.mLimitsMin = gradToPi( -119 ); //cMinAngle;
////	settings.mLimitsMax = gradToPi( 119 );//cMaxAngle;
//	PhysicsBody* parentBody = parentObject()->getProperty<PhysicsBody>();
//	PhysicsBody* childBody = childObject()->getProperty<PhysicsBody>();
//	assert( childBody->_ignoreCollisionsWith == null );
//	childBody->_ignoreCollisionsWith = parentBody;
//	JPH::Body* parentJolt = parentBody->body(); // JoltPhysics::Get()->_physics_system->GetBodyLockInterface().TryGetBody( parentBody->body_id() );
//	assert( parentJolt );
//	JPH::Body* childJolt = childBody->body(); //JoltPhysics::Get()->_physics_system->GetBodyLockInterface().TryGetBody( childBody->body_id() );
//	assert( childJolt );
//	JPH::TwoBodyConstraint* constraint = settings.Create( *parentJolt, *childJolt );
//	ASSERT( constraint );
//	_jph = asserted( dynamic_cast<JPH::HingeConstraint*>( constraint ) );
//	_jph->SetTargetAngularVelocity( 0.3f );
//	_jph->SetMotorState( JPH::EMotorState::Velocity );

//	_jph->SetTargetAngle( 0.3f );
//	_jph->SetMotorState( JPH::EMotorState::Position );

//	Physics::Get()->_physics_system->AddConstraint( constraint );


//}
//void PhysicsHingeJoint::onRemovedFromScene(Scene* scene){
//}
//void PhysicsHingeJoint::onParentPosOriChanged(){
//	if( _jph == null ){
//		reposChild();
//	}
//}
//void PhysicsHingeJoint::animate( float dt ){
//	OUNUSED( dt );
//	_rotation += Vec3( 0, 0, dt * 15 );

//}
//void PhysicsHingeJoint::reposChild(){
////	static int cc = 50;
////	if( cc > 0 ){
////		cc--;
////	} else {
////		return;
////	}

//	PosOri absPosOri( parentObject()->posori().matrix() * parentConnector()->type()->posori().matrix() );
//	PosOri childPosOri( absPosOri.matrix() * childConnector()->type()->posori().matrix().inverted() );
//	childPosOri.rotate( _rotation.z(), childPosOri.up() );
//	childObject()->setPosOri( childPosOri );
//}
