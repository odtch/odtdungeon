#include "stdodt.h"
#include "PhysicsShapes.h"
#include "JoltImpl.h"

#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/PolyhedronSubmergedVolumeCalculator.h>
#include "external/JoltPhysics/Jolt/Core/StreamWrapper.h"

//const char* PhysicsShapeType::Id = "PhysicsShapeType";
//PhysicsShapeType::PhysicsShapeType()
//	:ResourceType( 50, Id )
//{
//}
//PhysicsShapeType::~PhysicsShapeType(){
//}
//Resource* PhysicsShapeType::newInstance(){
//	return new PhysicsShape();
//}

PhysicsShape* PhysicsShape::Load( BinaryFileReader& reader ){
	PhysicsShape* shape = new PhysicsShape();
	shape->load( reader );
	return shape;
}

PhysicsShape* PhysicsShape::CreateCylinder( float radius, float width ){
	/// Create a shape centered around the origin
	/// with one top at (0, -inHalfHeight, 0) and
	/// the other at (0, inHalfHeight, 0)
	/// and radius inRadius.
	/// (internally the convex radius will be subtracted from the cylinder
	/// the total cylinder will not grow with the convex radius, but the edges of the cylinder will be rounded a bit).
	JPH::CylinderShapeSettings* cylindersettings = new JPH::CylinderShapeSettings( width / 2, radius );
	glm::quat q = Mat4::RotationAtAxis( -90, Vec3::Up ).getRotation();
	JPH::QuatArg rotation( q.x, q.y, q.z, q.w );
	JPH::RotatedTranslatedShapeSettings settings( Vec3::Null.toJPH(), rotation, cylindersettings );
	PhysicsShape* shape = CreateBySettings( settings );
	shape->_serializable = false;
	return shape;
}
PhysicsShape* PhysicsShape::CreateSphere(float radius){
	assert( 0.01f < radius );
	JPH::SphereShapeSettings settings( radius );
	return CreateBySettings( settings );
}
PhysicsShape* PhysicsShape::CreateBox( const Vec3& radius, float density ){
	assert( 0.01f < radius.x() ); assert( 0.01f < radius.y() ); assert( 0.01f < radius.z() );
	JPH::BoxShapeSettings settings(  radius.toJPH() );
	settings.mDensity = density;
	return CreateBySettings( settings );
}
PhysicsShape* PhysicsShape::CreateMesh( const Mesh<VertexPNT>& mesh ){
	JPH::VertexList vertices;
	vertices.reserve( mesh.vertexCount() );
	const VertexPNT* vp = &mesh.vertex( 0 );
	for( uint v = 0; v < mesh.vertexCount(); v++ ){
		vertices.push_back( JPH::Float3( vp->position.x, vp->position.y, vp->position.z ) );
		vp++;
	}
	JPH::IndexedTriangleList triangles;
	for( uint i = 0; i < mesh.indexCount(); i += 3 ){
		uint i0 = mesh.index( i + 0 );
		uint i1 = mesh.index( i + 1 );
		uint i2 = mesh.index( i + 2 );
		triangles.push_back( JPH::IndexedTriangle( i0, i1, i2 ) );
	}
	JPH::MeshShapeSettings settings(  vertices, triangles );
	return CreateBySettings( settings );
}
PhysicsShape* PhysicsShape::CreateConvexHull( const Mesh<VertexPNT>& mesh ){
	return CreateConvexHull( mesh, []( const VertexPNT& ){ return true; } );
}
PhysicsShape* PhysicsShape::CreateConvexHull( const Mesh<VertexPNT>& mesh, std::function<bool (const VertexPNT&)> filter ){
	JPH::Array<JPH::Vec3> points;
	const VertexPNT* vertex = &mesh.vertex( 0 );
	for( int v = 0; v < mesh.vertexCount(); v++ ){
		if( filter( *vertex ) ){
			points.push_back( JPH::Vec3( vertex->position.x, vertex->position.y, vertex->position.z ) );
		}
		vertex++;
	}
	PhysicsShape* shape = new PhysicsShape();
	{
		JPH::ConvexHullShapeSettings settings( points );
		// settings.mDensity *= 0.03f;
		JPH::ShapeSettings::ShapeResult shaperesult = settings.Create();
		assert( shaperesult.IsValid() );
		shape->_source = shaperesult.Get();
	} {
		shape->_centerofmassOffset = Vec3( shape->_source->GetCenterOfMass() ) * -1;
		JPH::OffsetCenterOfMassShapeSettings settings( shape->_centerofmassOffset.toJPH(), shape->_source );
		JPH::ShapeSettings::ShapeResult shaperesult = settings.Create();
		assert( shaperesult.IsValid() );
		shape->_jph = shaperesult.Get();
	}
	return shape;
}
PhysicsShape* PhysicsShape::CreateBySettings( JPH::ShapeSettings& settings ){
	JPH::ShapeSettings::ShapeResult shaperesult = settings.Create();
	assert( shaperesult.IsValid() );
	PhysicsShape* shape = new PhysicsShape();
	shape->_source = shaperesult.Get();
	shape->_jph = shape->_source;
	return shape;
}

PhysicsShape::PhysicsShape()
	:Resource( "PhysicsShape" )
{
}
PhysicsShape::~PhysicsShape(){
	_jph = null;
	_source = null;
}
bool PhysicsShape::castRay( const Vec3& from, const Vec3& to, Vec3& hitpoint ) const {
	Vec3 offset = to - from;
	//logDebug( "ps.cr", from, offset );
	ASSERT( 0 < offset.length() );
	JPH::RayCast inRay;
	inRay.mOrigin = from.toJPH();
	inRay.mDirection = offset.toJPH();
	JPH::SubShapeIDCreator inSubShapeIDCreator;
	JPH::RayCastResult ioHit;
	//ioHit.mFraction = offset.length2();
	assert( _jph );
	if( _jph->CastRay( inRay, inSubShapeIDCreator, ioHit ) ){
		//logDebug( "ps.cr h ", ioHit.mFraction );
		hitpoint = from + offset * ioHit.mFraction;
		return true;
	} else {
		return false;
	}
}
Box3 PhysicsShape::calcBoundingBox() const {
	assert( _jph );
	JPH::AABox jphbox = _jph->GetLocalBounds();
	return Box3( Vec3( jphbox.mMin ), Vec3( jphbox.mMax ) );
}
void PhysicsShape::save( BinaryFileWriter& writer ) const {
	Resource::save( writer );
	assert( _jph );
	writer.write_uint32( 317371 );
	assert( _serializable );
	writer.write_vec3( _centerofmassOffset );
	JPH::StreamOutWrapper stream( writer.asStd() );
	_source->SaveBinaryState( stream );
	writer.write_uint32( 317372 );
}
void PhysicsShape::load( BinaryFileReader& reader ){
	Resource::load( reader );
	assert( _jph == null );
	reader.read_magicnumber( 317371 );
	_centerofmassOffset = reader.read_vec3();
	JPH::StreamInWrapper stream( reader.asStd() );
	JPH::Shape::ShapeResult jphresult = JPH::Shape::sRestoreFromBinaryState( stream );
	if( jphresult.IsValid() == false ){
		othrow( "restored jph shape is invalid" );
	}
	reader.read_magicnumber( 317372 );
	_source = jphresult.Get();
	assert( _source );
	if( _centerofmassOffset.length2() < 0.00001f ){
		_centerofmassOffset.clear();
		_jph = _source;
	} else {
		JPH::OffsetCenterOfMassShapeSettings settings( _centerofmassOffset.toJPH(), _source );
		JPH::ShapeSettings::ShapeResult shaperesult = settings.Create();
		assert( shaperesult.IsValid() );
		_jph = shaperesult.Get();
	}
}


PhysicsMeshShapeBuilder::PhysicsMeshShapeBuilder( uint expectedVerticesCount, uint expectedTriangleCount ){
	if( 0 < expectedVerticesCount )
		_vertices.reserve( expectedVerticesCount );
	if( 0 < expectedTriangleCount )
		_triangles.reserve( expectedTriangleCount );
}
PhysicsMeshShapeBuilder::~PhysicsMeshShapeBuilder(){
}
void PhysicsMeshShapeBuilder::addVertex( const Vec3& position ){
	_vertices.push_back( JPH::Float3( position.x(), position.y(), position.z() ) );
}
void PhysicsMeshShapeBuilder::addTriangle( uint i0, uint i1, uint i2 ){
	_triangles.push_back( JPH::IndexedTriangle( i0, i1, i2 ) );
}
PhysicsShape* PhysicsMeshShapeBuilder::create(){
	JPH::MeshShapeSettings settings( _vertices, _triangles );
	return PhysicsShape::CreateBySettings( settings );
}
