#pragma once

#include "PhysicsCore.h"

//class PhysicsShapeType : public ResourceType
//{
//public:
//	static const char* Id;
//public:
//	explicit PhysicsShapeType();
//	virtual ~PhysicsShapeType() override;
//public:
//	virtual Resource* newInstance() override;
//};


class PhysicsShape : public Resource
{
	DEBUGCOUNTER( PhysicsShape )
	ODTNOCOPY( PhysicsShape )
public:
	static PhysicsShape* Load( BinaryFileReader& reader );
	static PhysicsShape* CreateCylinder( float radius, float width );
	static PhysicsShape* CreateSphere( float radius );
	static PhysicsShape* CreateBox( const Vec3& radius, float density = 1000.0f );
	static PhysicsShape* CreateMesh( const MeshPNT& mesh );
	static PhysicsShape* CreateConvexHull( const MeshPNT& mesh );
	static PhysicsShape* CreateConvexHull( const MeshPNT& mesh, std::function<bool( const VertexPNT& vertex )> filter );
	static PhysicsShape* CreateBySettings( JPH::ShapeSettings& jph );
private:
	JPH::Ref<JPH::Shape> _source;
	Vec3 _centerofmassOffset;
	bool _serializable = true;
	JPH::Ref<JPH::Shape> _jph;
private:
	explicit PhysicsShape();
public:
	virtual ~PhysicsShape() override;
public:
//	virtual ResourceType* type() const override { return Singleton::Get<PhysicsShapeType>(); }
public:
	JPH::Shape* jph() const{ ASSERT( _jph ); return _jph; }
public:
	bool castRay( const Vec3& from, const Vec3& to, Vec3& hitpoint ) const;
public:
	Box3 calcBoundingBox() const;
public:
	virtual void save( BinaryFileWriter& writer ) const override;
	virtual void load( BinaryFileReader& reader ) override;
public:
	friend class PhysicsShapeType;
	friend class PhysicsMeshShapeBuilder;
};

class PhysicsMeshShapeBuilder {
private:
	JPH::VertexList _vertices;
	JPH::IndexedTriangleList _triangles;
public:
	explicit PhysicsMeshShapeBuilder( uint expectedVerticesCount = 0, uint expectedTriangleCount = 0 );
	~PhysicsMeshShapeBuilder();
public:
	void addVertex( const Vec3& position );
	void addTriangle( uint i0, uint i1, uint i2 );
public:
	PhysicsShape* create();
};
