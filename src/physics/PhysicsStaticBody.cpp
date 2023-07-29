//#include "stdodt.h"
//#include "PhysicsStaticBody.h"
//#include "PhysicsShapes.h"
//#include "JoltPhysics.h"
//#include "JoltImpl.h"

//PhysicsStaticBodyType::PhysicsStaticBodyType( ObjectType* object, PhysicsShape* shape )
//	:ObjectPropertyType( object )
//	,_shape( asserted( shape ) )
//{
//}
//PhysicsStaticBodyType::~PhysicsStaticBodyType(){
//}
//void PhysicsStaticBodyType::newInstance( Object* object ){
//	new PhysicsStaticBody( object, this );
//}

//PhysicsStaticBody::PhysicsStaticBody( Object* object, PhysicsStaticBodyType* type )
//	:ObjectProperty( object, type )
//{
//}
//PhysicsStaticBody::~PhysicsStaticBody(){
//	ASSERT( _jolt == null );
//	ASSERT( _body_id == null );
//}
//void PhysicsStaticBody::onAddedToScene( Scene* scene ){
//	OUNUSED( scene );
//	assert( _jolt == null );
//	_jolt = JoltPhysics::Get();
//	const PosOri& posori = object()->posori();
//	JPH::Vec3 jpos( posori.position().x(), posori.position().y(), posori.position().z() );
//	glm::quat r = glm::quat_cast( posori.matrix().asGlm() );
//	JPH::Quat jrot( r.x, r.y, r.z, r.w );
//	JPH::BodyCreationSettings sphere_settings( type()->shape()->jph(), jpos, jrot, JPH::EMotionType::Static, 0 /* Layers::NON_MOVING*/ );
//	assert( _body_id == null );
//	JPH::BodyID body_id = _jolt->_body_interface->CreateAndAddBody( sphere_settings, JPH::EActivation::DontActivate );
//	_body_id = new JPH::BodyID( body_id );
//}
//void PhysicsStaticBody::onRemovedFromScene( Scene* scene ){
//	OUNUSED( scene );
//	if( _body_id == null ){
//		ASSERT( false );
//	} else {
//		ASSERT( _jolt );
//		_jolt->_body_interface->RemoveBody( *_body_id );
//		_jolt->_body_interface->DestroyBody( *_body_id );
//		odelete( _body_id );
//	}
//	_jolt = null;
//}
