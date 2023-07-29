#pragma once

#include "PhysicsCore.h"

//class PhysicsFollower
//{
//	DEBUGCOUNTER( PhysicsFollower )
//	ODTNOCOPY( PhysicsFollower )
//private:
//	Object* _centerObject = null;
//	Vec3 _centerOffsetInObject;
//	Vec3 _centerOffsetInWorld;
//private:
//	PosOri _centerPosOri;
//private:
//	Vec2 _rotation;
//	float _distance = 30;
//	float _minDistance = 1;
//private:
//	float _damping = 0.95f;
//private:
//	PosOri _expectedPosOri;
//private:
//	float _colliderRadius = -1;
//	PhysicsShape* _colliderShape = null;
//	PosOri _rayPosOri;
//	PhysicsShapeCaster* _rayCaster = null;
//private:
//	float _solverRotSpeed = 0;
//	float _solverRotation_x = 0;
//	PosOri _solverPosOri;
//private:
//	Vec3 _currentTarget;
//	PosOri _currentPosOri;
//public:
//	explicit PhysicsFollower( float colliderRadius );
//	virtual ~PhysicsFollower();
//public:
//	bool hasCenterObject() const{ return _centerObject != null; }
//	Object* centerObject() const{ return asserted( _centerObject ); }
//	void setCenterObject( Object* centerObject );
//	void setCenterObjectOffset( const Vec3& offsetInObject, const Vec3& offsetInWorld );
//public:
//	void setCenter( const PosOri& posori );
//public:
//	const Vec2& rotation() const{ return _rotation; }
//	void setRotation( const Vec2& rotation );
//	float distance() const{ return _distance; }
//	void setDistance( float distance );
//	float minDistance() const{ return _minDistance; }
//	void setMinDistance( float minDistance );
//public:
//	float damping() const{ return _damping; }
//	void setDamping( float damping );
//public:
//	const PosOri& currentPosOri() const{ return _currentPosOri; }
//public:
//	virtual void update( float dt );
//	virtual void debug( MeshPNT& mesh );
//};

////class PhysicsRayCaster : public PhysicsFollower
////{
////	DEBUGCOUNTER( PhysicsRayCaster )
////	ODTNOCOPY( PhysicsRayCaster )
////private:
////public:
////	explicit PhysicsRayCaster();
////	virtual ~PhysicsRayCaster();
////public:
////private:
////	virtual void cast() override;
////};


//class PhysicsFollowerDebugger : public Object
//{
//private:
//	PhysicsFollower* _follower;
//	bool _ownAndUpdateFollower;
//private:
//	MeshPNT _debugMesh;
//	Material _debugMaterial;
//public:
//	explicit PhysicsFollowerDebugger( bool ownAndUpdateFollower, PhysicsFollower* follower );
//	virtual ~PhysicsFollowerDebugger();
//public:
//	virtual void animate( float dt ) override;
//};
