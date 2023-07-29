#include "stdodt.h"
#include "PhysicsBody.h"
#include "PhysicsShapes.h"
#include "Physics.h"
#include "JoltImpl.h"

//	_physics->onBodyDestroyed( this );
//	ASSERT( _body_id == null );
//}
////		//JPH::Vec3 position = _physics->_body_interface->GetCenterOfMassPosition( *_body_id );
////		JPH::Mat44 m = _physics->_body_interface->GetCenterOfMassTransform( *_body_id );
////		float & d = m(0,0);
////		Mat4 t;
////		float& td = t(0,0);
////		float* dp = &d;
////		float* tp = &td;
////		for( int i = 0; i < 4 * 4; i++ ){
////			(*tp) = (*dp);
////			logDebug( i, (*tp) );
////			tp++; dp++;
////		}
////		PosOri po( t );
////		_posori->set( po );
////		//::Vec3 p( position[0], position[1], position[2] );
////		//_posori->set( PosOri( p, _posori->get().orientation() ) );


//PhysicsBodyType::PhysicsBodyType( ObjectType* object, PhysicsShape* shape, PhysicsMotionType type )
//	:ObjectPosOriPropertyType( object )
//	,_shape( asserted( shape ) )
//	,_motiontype( type )
//{
//}
//PhysicsBodyType::~PhysicsBodyType(){
//}
//ObjectProperty* PhysicsBodyType::newInstance( Object* object, ObjectPosOriProperty* parent ){
//	return new PhysicsBody( this, parent, object );
//}

//PhysicsBody::PhysicsBody( PhysicsBodyType* type, ObjectPosOriProperty* parent, Object* object)
//	:ObjectPosOriProperty( object, parent, type )
//{
//	assert( type );
//	_shape = type->shape();
//	_motiontype = type->motiontype();
//	assert( _shape );
//	assert( _motiontype );
//}
PhysicsBody::PhysicsBody(PhysicsShape* shape, PhysicsMotionType motiontype, SceneObject* object)
//PhysicsBody::PhysicsBody(PhysicsShape* shape, PhysicsMotionType motiontype, ObjectPosOriProperty* parent, Object* object )
	:SceneProperty( object )
	,_object( asserted( object ) )
	,_shape( asserted( shape ) )
	,_motiontype( asserted( motiontype ) )
{
}
PhysicsBody::~PhysicsBody(){
	ASSERT( _physics == null );
	ASSERT( _jph == null );
	_object = null;
	_shape = null;
}
//void PhysicsBody::setMotionType( PhysicsMotionType motiontype ){
//	assert( motiontype );
//	assert( _jph == null );
//	_motiontype = motiontype;
//}
//void PhysicsBody::moveKinematic( const PosOri& posori ){
//	assert( _motiontype == PhysicsMotionType_Kinematic );
//	Mat4 m = posori.orientation().matrix();
//	glm::quat q = m.getRotation();
//	JPH::Quat r( q.x, q.y, q.z, q.w );
//	assert( _jph );
//	_jph->MoveKinematic( posori.position().toJPH(), r, 0.10f );
//	_physics->_body_interface->ActivateBody( body()->GetID() );
//}
void PhysicsBody::onAddedToScene( Scene* scene ){
	OUNUSED( scene );
	assert( _physics == null );
	_physics = Physics::Get();
	PosOri posori = _object->posori();// calcPosOriInWorldFromParent();
	//logDebug( "PhysicsBody::create ", posori.position() );
	JPH::Vec3 jpos( posori.position().x(), posori.position().y(), posori.position().z() );
	glm::quat r = glm::quat_cast( posori.matrix().asGlm() );
	JPH::Quat jrot( r.x, r.y, r.z, r.w );
	JPH::EMotionType emotion;
	JPH::ObjectLayer objectlayer;
	JPH::EActivation activationMode;
	if( _motiontype == PhysicsMotionType_Dynamic ){
		emotion = JPH::EMotionType::Dynamic;
		objectlayer = 1; // Layers::MOVING
		activationMode = JPH::EActivation::Activate;
	} else if( _motiontype == PhysicsMotionType_Static ){
		emotion = JPH::EMotionType::Static;
		objectlayer = 0; // Layers::NON_MOVING
		activationMode = JPH::EActivation::DontActivate;
	} else if( _motiontype == PhysicsMotionType_Kinematic ){
		emotion = JPH::EMotionType::Kinematic;
		objectlayer = 1; // Layers::NON_MOVING
		activationMode = JPH::EActivation::DontActivate;
	} else {
		assert( false );
	}
	assert( _shape );
	JPH::BodyCreationSettings bodyCreationSettings( _shape->jph(), jpos, jrot, emotion, objectlayer );
	bodyCreationSettings.mUserData = (uint64_t) this;
	//JPH::BodyID body_id = _jolt->_body_interface->CreateAndAddBody( bodyCreationSettings, activationMode );
	assert( _jph == null );
	_jph = _physics->_body_interface->CreateBody( bodyCreationSettings );
	assert( _jph );
	_physics->_body_interface->AddBody( _jph->GetID(), activationMode );
	//_body_id = new JPH::BodyID( body_id );
	//	Now you can interact with the  body, in this case we're going to give it a velocity.
	//	(note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
	//	_body_interface->SetLinearVelocity( body_id, JPH::Vec3(0.0f, 0.0f, 5.0f ) );
	//	auto body = new PhysicsBody( body_id, posori, this );
	if( _motiontype == PhysicsMotionType_Dynamic
		|| _motiontype == PhysicsMotionType_Kinematic
		){
		_physics->_bodies.add( this );
	}
}
void PhysicsBody::onRemovedFromScene( Scene* scene ){
	OUNUSED( scene );
	if( _jph == null ){
		ASSERT( false );
	} else {
		ASSERT( _physics );
		if( _motiontype == PhysicsMotionType_Dynamic
			|| _motiontype == PhysicsMotionType_Kinematic
			){
			_physics->_bodies.remove( this );
		}
		_physics->_body_interface->RemoveBody( _jph->GetID() );
		_physics->_body_interface->DestroyBody( _jph->GetID() );
		//odelete( _body_id );
		_jph = null;
	}
	_physics = null;
}
void PhysicsBody::animate( float dt ){
	//ObjectPosOriProperty::animate( dt );
	assert( _jph );
//	PosOri po;
//	setPosOriFromJPH( po, _jph->GetCenterOfMassTransform() );
//	//logDebug( "pb", po.position() );
//	//setPosOri( po );
//	int todo_relative;
//	this->setPosOri( po );
//	object()->setPosOri( po );
}
void PhysicsBody::onPhysicsUpdated(){
	assert( _jph );
	if( _motiontype == PhysicsMotionType_Dynamic ){
		PosOri posori;
		setPosOriFromJPH( posori, _jph->GetCenterOfMassTransform() );
		ASSERT( _object );
		//logDebug( "pb", posori.position() );
		_object->setPosOri( posori );
	}
}
