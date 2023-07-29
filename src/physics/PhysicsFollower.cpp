#include "stdodt.h"
#include "PhysicsFollower.h"
#include "Physics.h"
#include "PhysicsShapes.h"
#include "JoltImpl.h"
#include "external/JoltPhysics/Jolt/Physics/Collision/ShapeCast.h"

//PhysicsFollower::PhysicsFollower( float colliderRadius )
//	:_colliderRadius( colliderRadius )
//{
//	assert( 0.001f < _colliderRadius );
//	_colliderShape = PhysicsShape::CreateSphere( _colliderRadius );
//	_rayCaster = new PhysicsShapeCaster( _colliderShape );
//}
//PhysicsFollower::~PhysicsFollower(){
//	odelete( _rayCaster );
//	odelete( _colliderShape );
//}
//void PhysicsFollower::setCenterObject( Object* centerObject ){
//	_centerObject = centerObject;
//	_rayCaster->setFilterExcludeBody( null );
//	if( _centerObject ){
//		PhysicsBody* body = _centerObject->findProperty<PhysicsBody>();
//		if( body ){
//			_rayCaster->setFilterExcludeBody( body );
//		}
//	}
//}
//void PhysicsFollower::setCenterObjectOffset( const Vec3& offsetInObject, const Vec3& offsetInWorld ){
//	_centerOffsetInObject = offsetInObject;
//	_centerOffsetInWorld = offsetInWorld;
//}
//void PhysicsFollower::setCenter( const PosOri& posori ){
//	assert( !hasCenterObject() );
//	_centerPosOri = posori;
//}
//void PhysicsFollower::setRotation( const Vec2& rotation ){
//	_rotation = rotation;
//}
//void PhysicsFollower::setDistance( float distance ){
//	assert( 0.1f < distance );
//	_distance = distance;
//}
//void PhysicsFollower::setMinDistance( float minDistance ){
//	assert( 0 < minDistance );
//	_minDistance = minDistance;
//}
//void PhysicsFollower::setDamping( float damping ){
//	assert( 0 <= damping && damping < 10 );
//}
//void PhysicsFollower::update( float dt ){
//	if( _centerObject ){
//		Vec3 targetDirection = _centerObject->posori().direction().zed( 0 );
//		if( targetDirection.length2() < 0.001 ){
//			logError( "Following target direction z >1" );
//		} else {
//			targetDirection.normalize();
//			_centerPosOri.set( _centerObject->position(), targetDirection, Vec3::Up );
//			_centerPosOri.translateLocal( _centerOffsetInObject );
//			_centerPosOri.translate( _centerOffsetInWorld );
//		}
//	}{
//		_expectedPosOri = _centerPosOri;
//		_expectedPosOri.rotate( _rotation.x(), _expectedPosOri.right() );
//		_expectedPosOri.rotate( _rotation.y(), Vec3::Up );
//		//_expectedPosOri.rotate( _rotation.x() + _collisionRotationX, _expectedPosOri.right() );
//		_expectedPosOri.translate( _expectedPosOri.direction() * -_distance );
//	}{
//		bool _prevHasCollistion = _rayCaster->hasHit();
//		int _prevSolverRotation_x = _solverRotation_x;
//		_solverRotSpeed += 0.51f * dt;
//		if( _prevHasCollistion == false ){
//			_solverRotation_x -= _solverRotSpeed;
//		} else {
//			_solverRotation_x += _solverRotSpeed;
//		}
//		if( _solverRotation_x < 0 )_solverRotation_x = 0;
//		if( _solverRotation_x > 85 - _rotation.x() )_solverRotation_x = 85  - _rotation.x();
//		_rayPosOri = _centerPosOri;
//		_rayPosOri.rotate( _rotation.x(), _rayPosOri.right() );
//		_rayPosOri.rotate( _rotation.y(), Vec3::Up );
//		_rayPosOri.rotate( _solverRotation_x, _rayPosOri.right() );
//		_rayCaster->update( _rayPosOri.position(), -_rayPosOri.direction(), _distance );
//		if( _rayCaster->hasHit() == false ){
//		} else {
//			if( _prevHasCollistion == false ){
//				_solverRotation_x = _prevSolverRotation_x;
//				_solverRotSpeed *= 0.5f;
////				_solverRotation_x -= _solverRotSpeed;
////				if( _solverRotation_x < 0 )_solverRotation_x = 0;
//			} else {
//			}
//		}
//		_solverPosOri = _centerPosOri;
//		_solverPosOri.rotate( _rotation.x(), _solverPosOri.right() );
//		_solverPosOri.rotate( _rotation.y(), Vec3::Up );
//		_solverPosOri.rotate( _solverRotation_x, _solverPosOri.right() );
//		_solverPosOri.translate( _solverPosOri.direction() * -_distance );
//	}{
//		float fc = OdtMath::adjusted( _damping * dt, 0, 1 );
//		float fs = 1.0f - fc;
//		_currentTarget = _currentTarget * fc + _centerPosOri.position() * fs;
//		Vec3 _currentPos = _currentPosOri.position() * fc + _solverPosOri.position() * fs;
//		Vec3 _curDir = ( _currentTarget - _currentPos ).normalized();
//		Vec3 _curUp = Vec3::Cross( _solverPosOri.right(), _curDir );
//		//_currentPosOri = PosOri::Interpolated( _currentPosOri, _solverPosOri, OdtMath::adjusted( 1.0f - _damping * dt, 0.00001f, 1 ) );
//		_currentPosOri.set( _currentPos, _curDir, _curUp );
//	}
//}
//void PhysicsFollower::debug( Mesh<VertexPNT>& mesh ){
//	MeshBuilder::CreateBox( mesh, _centerPosOri, Vec3( 0.4f, 0.7f, 0.2f ), VertexPNT( Vec3::Null, Vec3( 0, 0, 0 ), Vec2::Null ) );
//	MeshBuilder::CreateBox( mesh, _expectedPosOri, Vec3( 0.4f, 0.7f, 0.2f ), VertexPNT( Vec3::Null, Vec3( 0, 0, 0 ), Vec2::Null ) );
//	MeshBuilder::CreateLine( mesh, _expectedPosOri.position(), _centerPosOri.position(), 0.05f, 6, true, VertexPNT() );
//	if( _rayCaster->hasHit() ){
//		MeshBuilder::CreateSphere( mesh, _rayCaster->hitPosition(), _colliderRadius, 2, VertexPNT() );
//		float length = _colliderRadius * 1.5f;
//		float radius = 0.15f;
//		Vec3 center = _rayCaster->contactPoint();
//		MeshBuilder::CreateLine( mesh, center - Vec3::Right * length, center + Vec3::Right * length, radius, 6, true, VertexPNT() );
//		MeshBuilder::CreateLine( mesh, center - Vec3::Forward * length, center + Vec3::Forward * length, radius, 6, true, VertexPNT() );
//		MeshBuilder::CreateLine( mesh, center - Vec3::Up * length, center + Vec3::Up * length, radius, 6, true, VertexPNT() );

//		MeshBuilder::CreateBox( mesh, _solverPosOri, Vec3( 0.4f, 0.7f, 0.2f ), VertexPNT( Vec3::Null, Vec3( 0, 0, 0 ), Vec2::Null ) );
//		MeshBuilder::CreateLine( mesh, _solverPosOri.position(), _centerPosOri.position(), 0.05f, 6, true, VertexPNT() );
//	}

//	MeshBuilder::CreateBox( mesh, _currentPosOri, Vec3( 0.4f, 0.7f, 0.2f ), VertexPNT( Vec3::Null, Vec3( 0, 0, 0 ), Vec2::Null ) );
//	MeshBuilder::CreateLine( mesh, _currentPosOri.position(), _centerPosOri.position(), 0.15f, 6, true, VertexPNT() );

//	//	MeshBuilder::CreateSphere( mesh, _currentPosition, _colliderRadius, 1, VertexPNT() );
////	MeshBuilder::CreateLine( mesh, _currentPosition, _targetPosOri.position(), 0.2f, 6, true, VertexPNT() );
////	if( ( _currentPosition - _expectedPosOri.position() ).length2() > 0.02f ){
////		//MeshBuilder::CreateLine( mesh, _currentPosition, _expectedPosOri.position(), 0.2f, 6, true, VertexPNT() );
////	}
//}



//PhysicsFollowerDebugger::PhysicsFollowerDebugger( bool updateFollower, PhysicsFollower* follower )
//	:_follower( asserted( follower ) )
//	,_ownAndUpdateFollower( updateFollower )
//{
//	//_debugMaterial.setFlag( MaterialFlag_UseNormalAsColor );
//	//_debugMaterial.setFlag( MaterialFlag_NoLightAffected );
//	//_debugMaterial.setFlag( MaterialFlag_CalcNormalFromTriangle );
//	_debugMaterial.setFlag( MaterialFlag_NoShadowEmit );
//	//_debugMaterial.setColor( vec4( 0.03f, 0.03f, 0.03f, 0.03f ) );
//	_debugMaterial.setColor( vec4( 0.3f, 0.3f, 1.0f, 1 ) );
//	MeshBuilder::CreateSphere( _debugMesh, Vec3::Null, 0.1f, 1, VertexPNT() );
//	ObjectRenderableProperty* renderer = new ObjectRenderableProperty( &_debugMesh, &_debugMaterial, this );
//	renderer->setAttachementType( ObjectPosOriAttachement::Dynamic );
//}
//PhysicsFollowerDebugger::~PhysicsFollowerDebugger(){
//	if( _ownAndUpdateFollower ){
//		odelete( _follower );
//	}
//}
//void PhysicsFollowerDebugger::animate( float dt ){
//	if( _ownAndUpdateFollower ){
//		_follower->update( dt );
//	}
//	Object::animate( dt );
//	_debugMesh.clear();
//	_follower->debug( _debugMesh );
//	if( _debugMesh.isEmpty() )
//		MeshBuilder::CreateSphere( _debugMesh, Vec3::Null, 0.1f, 1, VertexPNT() );
//}
