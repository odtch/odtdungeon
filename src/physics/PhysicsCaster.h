#pragma once

#include "PhysicsCore.h"

//class PhysicsCaster
//{
//	DEBUGCOUNTER( PhysicsCaster )
//	ODTNOCOPY( PhysicsCaster )
//protected:
//	Vec3 _origin;
//	Vec3 _direction;
//	float _range = 10000;
//protected:
//	JPH::BroadPhaseLayerFilter* _broadPhaseLayerFilter = null;
//	JPH::ObjectLayerFilter* _objectLayerFilter = null;
//	PhysicsBody* _excludeBody = null;
//	JPH::BodyFilter* _bodyFilter = null;
//protected:
//	bool _hasHit = false;
//	float _hitDistance = 0;
//	Vec3 _hitPosition;
//	JPH::BodyID _bodyId;
//	//Object* _hitObject = null;
//public:
//	explicit PhysicsCaster();
//	virtual ~PhysicsCaster();
//public:
//	const Vec3& origin() const{ return _origin; }
//	void setOrigin( const Vec3& origin );
//	const Vec3& direction() const{ return _direction; }
//	void setDirection( const Vec3& direction );
//	Vec3 pointInDistance( float d ) const{ return _origin + _direction * d; }
//	float range() const{ return _range; }
//	void setRange( float range );
//public:
//	void setFilterExcludeBody( PhysicsBody* body );
//public:
//	void clear();
//	void update();
//	void update( const Vec3& origin, const Vec3& direction );
//	void update( const Vec3& origin, const Vec3& direction, float range );
//	void update( const PosOri& posori, float range );
//	void update( const Ray& ray );
//protected:
//	virtual void cast() = 0;
//public:
//	bool hasHit() const{ return _hasHit; }
//	float hitDistance() const { ASSERT( _hasHit ); return _hitDistance; }
//	const Vec3& hitPosition() const { ASSERT( _hasHit ); return _hitPosition; }
////	bool hasHitObject() const;
////	Object* hitObject() const;
//	PhysicsBody* findHitBody();
//	SceneObject* findHitObject();
//};

//class PhysicsRayCaster : public PhysicsCaster
//{
//	DEBUGCOUNTER( PhysicsRayCaster )
//	ODTNOCOPY( PhysicsRayCaster )
//private:
//	JPH::RayCastResult _jph_result;
//public:
//	explicit PhysicsRayCaster();
//	virtual ~PhysicsRayCaster();
//public:
//	Vec3 calcHitNormal();
//protected:
//	virtual void cast() override;
//};


//class PhysicsShapeCaster : public PhysicsCaster
//{
//	ODTNOCOPY( PhysicsShapeCaster )
//private:
//	PhysicsShape* _shape = null;
//private:
//	Vec3 _contactPoint;
//public:
//	explicit PhysicsShapeCaster();
//	explicit PhysicsShapeCaster( PhysicsShape* shape );
//	virtual ~PhysicsShapeCaster();
//public:
//	void setShape( PhysicsShape* shape );
//public:
//	const Vec3& contactPoint() const{ ASSERT( hasHit() ); return _contactPoint; }
//protected:
//	virtual void cast() override;
//};
