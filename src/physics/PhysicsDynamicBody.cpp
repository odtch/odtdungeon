//#include "stdodt.h"
//#include "PhysicsDynamicBody.h"
//#include "PhysicsShapes.h"
//#include "JoltPhysics.h"
//#include "JoltImpl.h"

////	_physics->onDynamicBodyDestroyed( this );
////	ASSERT( _body_id == null );
////}
//////		//JPH::Vec3 position = _physics->_body_interface->GetCenterOfMassPosition( *_body_id );
//////		JPH::Mat44 m = _physics->_body_interface->GetCenterOfMassTransform( *_body_id );
//////		float & d = m(0,0);
//////		Mat4 t;
//////		float& td = t(0,0);
//////		float* dp = &d;
//////		float* tp = &td;
//////		for( int i = 0; i < 4 * 4; i++ ){
//////			(*tp) = (*dp);
//////			logDebug( i, (*tp) );
//////			tp++; dp++;
//////		}
//////		PosOri po( t );
//////		_posori->set( po );
//////		//::Vec3 p( position[0], position[1], position[2] );
//////		//_posori->set( PosOri( p, _posori->get().orientation() ) );


//PhysicsDynamicBodyType::PhysicsDynamicBodyType(ObjectType* object, PhysicsShape* shape)
//	:ObjectPropertyType( object )
//	,_shape( asserted( shape ) )
//{
//}
//PhysicsDynamicBodyType::~PhysicsDynamicBodyType(){
//}
//void PhysicsDynamicBodyType::newInstance( Object* object ){
//	new PhysicsDynamicBody( object, this );
//}

//PhysicsDynamicBody::PhysicsDynamicBody(Object* object, PhysicsDynamicBodyType* type)
//	:ObjectProperty( object, type )
//{
//}
//PhysicsDynamicBody::~PhysicsDynamicBody(){
//	ASSERT( _jolt == null );
//	ASSERT( _body_id == null );
//}
//void PhysicsDynamicBody::onAddedToScene( Scene* scene ){
//	OUNUSED( scene );
//	assert( _jolt == null );
//	_jolt = JoltPhysics::Get();
//	const PosOri& posori = object()->posori();
//	JPH::Vec3 jpos( posori.position().x(), posori.position().y(), posori.position().z() );
//	glm::quat r = glm::quat_cast( posori.matrix().asGlm() );
//	JPH::Quat jrot( r.x, r.y, r.z, r.w );
//	JPH::BodyCreationSettings bodyCreationSettings( type()->shape()->jph(), jpos, jrot, JPH::EMotionType::Dynamic, 1 /* Layers::MOVING */ );
//	//assert( bodyCreationSettings.HasMassProperties() );
//	assert( _body_id == null );
//	JPH::BodyID body_id = _jolt->_body_interface->CreateAndAddBody( bodyCreationSettings, JPH::EActivation::Activate );
//	_body_id = new JPH::BodyID( body_id );
//	//	Now you can interact with the dynamic body, in this case we're going to give it a velocity.
//	//	(note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
//	//	_body_interface->SetLinearVelocity( body_id, JPH::Vec3(0.0f, 0.0f, 5.0f ) );
//	//	auto body = new PhysicsDynamicBody( body_id, posori, this );
//	_jolt->_bodies.add( this );
//}
//void PhysicsDynamicBody::onRemovedFromScene( Scene* scene ){
//	OUNUSED( scene );
//	if( _body_id == null ){
//		ASSERT( false );
//	} else {
//		ASSERT( _jolt );
//		_jolt->_bodies.remove( this );
//		_jolt->_body_interface->RemoveBody( *_body_id );
//		_jolt->_body_interface->DestroyBody( *_body_id );
//		odelete( _body_id );
//	}
//	_jolt = null;
//}
//void PhysicsDynamicBody::onPhysicsUpdated(){
//	assert( _body_id );
//	PosOri po;
//	setPosOriFromJPH( po, _jolt->_body_interface->GetCenterOfMassTransform( *_body_id ) );
//	object()->setPosOri( po );
//}
