#include "stdodt.h"
#include "PhysicsCaster.h"
//#include "Physics.h"
//#include "PhysicsShapes.h"
//#include "JoltImpl.h"
//#include "external/JoltPhysics/Jolt/Physics/Collision/ShapeCast.h"
//#include "math/Ray.h"

//PhysicsCaster::PhysicsCaster(){
//}
//PhysicsCaster::~PhysicsCaster(){
//	odelete( _bodyFilter );
//}
//void PhysicsCaster::setOrigin( const Vec3& origin ){
//	_origin = origin;
//	clear();
//}
//void PhysicsCaster::setDirection( const Vec3& direction ){
//	_direction = direction;
//	clear();
//}
//void PhysicsCaster::setRange( float range ){
//	assert( 1 < range );
//	_range = range;
//	clear();
//}
//void PhysicsCaster::setFilterExcludeBody( PhysicsBody* body ){
//	if( _excludeBody == body )
//		return;
//	_excludeBody = body;
//	if( body == null ){
//		if( _bodyFilter ){
//			delete _bodyFilter;
//			_bodyFilter = null;
//		}
//	} else {
//		if( _bodyFilter ){
//			delete _bodyFilter;
//		}
//		assert( body->_jph );
//		_bodyFilter = new JPH::IgnoreSingleBodyFilter( body->_jph->GetID() );
//	}
//}
//void PhysicsCaster::clear(){
//	_hasHit = false;
//}
//void PhysicsCaster::update(){
//	_hasHit = false;
//	if( _broadPhaseLayerFilter == null )
//		_broadPhaseLayerFilter = new JPH::BroadPhaseLayerFilter();
//	if( _objectLayerFilter == null )
//		_objectLayerFilter = new JPH::ObjectLayerFilter();
//	if( _bodyFilter == null )
//		_bodyFilter = new JPH::BodyFilter();
//	/*
//	const BroadPhaseLayerFilter &inBroadPhaseLayerFilter = { },
//			const ObjectLayerFilter &inObjectLayerFilter = { },
//			const BodyFilter &inBodyFilter = { }
//											 ) const;
//	*/
//	cast();
//}
//void PhysicsCaster::update( const Vec3& origin, const Vec3& direction ){
//	_origin = origin;
//	_direction = direction;
//	update();
//}
//void PhysicsCaster::update( const Vec3& origin, const Vec3& direction, float range ){
//	_origin = origin;
//	_direction = direction;
//	_range = range;
//	update();
//}
//void PhysicsCaster::update( const PosOri& posori, float range ){
//	update( posori.position(), posori.direction(), range );
//}
//void PhysicsCaster::update( const Ray& ray ){
//	update( ray.origin(), ray.direction() );
//}
////PhysicsBody* PhysicsCaster::findHitBody(){
////	ASSERT( hasHit() );
////	void* ptr = (void*) Physics::Get()->_body_interface->GetUserData( _bodyId );
////	if( ptr == null )
////		return null;
////	PhysicsBody* body = (PhysicsBody*)( ptr );
////	assert( body );
////	return body;
////}
////SceneObject* PhysicsCaster::findHitObject(){
////	PhysicsBody* body = findHitBody();
////	if( body == null )
////		return null;
////	return body->object();
////}

//PhysicsRayCaster::PhysicsRayCaster(){
//}
//PhysicsRayCaster::~PhysicsRayCaster(){
//}
////Vec3 PhysicsRayCaster::calcHitNormal(){
////	// If you want the surface normal of the hit use
////	// Body::GetWorldSpaceSurfaceNormal(collected sub shape ID, inRay.GetPointOnRay(collected fraction))
////	// on body with collected body ID.
////	ASSERT( hasHit() );
////	PhysicsBody* body = findHitBody();
////	if( body == null )
////		return - direction();
////	return Vec3( body->_jph->GetWorldSpaceSurfaceNormal( _jph_result.mSubShapeID2, _hitPosition.toJPH() ) );
////}
////void PhysicsRayCaster::cast(){
////	const JPH::NarrowPhaseQuery& narrowPhaseQuery = Physics::Get()->_physics_system->GetNarrowPhaseQueryNoLock();
////	JPH::RayCast r;
////	r.mOrigin = _origin.toJPH();
////	ASSERT( 0.0001f < _range );
////	r.mDirection = ( _direction * _range ).toJPH();
////	JPH::RRayCast inRay( r );
////	_jph_result = JPH::RayCastResult();
////	if( narrowPhaseQuery.CastRay(
////		inRay, _jph_result, *_broadPhaseLayerFilter, *_objectLayerFilter, *_bodyFilter
////	) ){
////		_hitDistance = _jph_result.mFraction * _range;
////		_hitPosition = _origin + _direction * _hitDistance;
////		_hasHit = true;
////		_bodyId = _jph_result.mBodyID;
////	}
////}

//PhysicsShapeCaster::PhysicsShapeCaster(){
//}
//PhysicsShapeCaster::PhysicsShapeCaster( PhysicsShape* shape )
//	:_shape( asserted( shape ) )
//{
//}
//PhysicsShapeCaster::~PhysicsShapeCaster(){
//}
//void PhysicsShapeCaster::setShape( PhysicsShape* shape ){
//	_shape = asserted( shape );
//	clear();
//}
////void PhysicsShapeCaster::cast(){
////	assert( _shape );
////	const JPH::NarrowPhaseQuery& narrowPhaseQuery = Physics::Get()->_physics_system->GetNarrowPhaseQueryNoLock();
////	static JPH::Vec3 scale( 1, 1, 1 );
////	JPH::RShapeCast shapeCast {
////		_shape->jph(),
////		scale,
////		//JPH::RMat44::sTranslation( JPH::RVec3( 0, 0, 0 ) ),
////		JPH::RMat44::sTranslation( _origin.toJPH() ),
////		_direction.toJPH() * _range
////	};
////	//shapeCast.mDirection
////	// RShapeCast shape_cast { normal_sphere, Vec3::sReplicate(1.0f), RMat44::sTranslation(RVec3(0, 11, 0)), Vec3(0, 1, 0) };

////	// ShapeCastT(const Shape *inShape, Vec3Arg inScale, typename Mat::ArgType inCenterOfMassStart, Vec3Arg inDirection) :
////	JPH::ShapeCastSettings shapeCastSettings;
////	JPH::Vec3 baseOffset;
////	JPH::AllHitCollisionCollector<JPH::CastShapeCollector> collector;
////	narrowPhaseQuery.CastShape( shapeCast, shapeCastSettings, baseOffset, collector, *_broadPhaseLayerFilter, *_objectLayerFilter, *_bodyFilter );
////	if( collector.HadHit() ){
////		for( int h = 0; h < collector.mHits.size(); h++ ){
////			const JPH::ShapeCastResult& hit = collector.mHits[h];
////			float thisHitDistance = hit.mFraction * _range;
////			ASSERT( h == 0 || _hitDistance >= thisHitDistance );
////			_hitDistance = thisHitDistance;
////			_hitPosition = _origin + _direction * _hitDistance;
////			_contactPoint = Vec3( hit.mContactPointOn1 );
////			_hasHit = true;
////			return;
////		}
////	}
//////			_hitDistance = _range * ioHit.mFraction;
//////			_hitPoint = Vec3( inRay.GetPointOnRay( ioHit.mFraction ) );
//////			_hasHit = true;
////}
