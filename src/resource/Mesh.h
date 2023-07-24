#pragma once

#include "ResourceCore.h"
#include "Resource.h"
//#include "ResourceType.h"
//#include "ResourceStorage.h"
#include "Vertex.h"
//#include "space/vulkan/resource/VulkanMeshBuffer.h"

class AbstractMesh : public Resource
{
	DEBUGCOUNTER(AbstractMesh)
public:
	enum ModificationState {
		NotModified,
		ModifiedInCPU,
		ModifiedInGPU
	};
protected:
	ModificationState _modificationState;
//public:
//	enum UpdateType {
//		UpdateImmediate,
//		UpdateBackground
//	};
//private:
//	UpdateType _updateType = UpdateImmediate;
//private:
//	VkGeometryFlagsKHR _geometryFlags =
//			VK_GEOMETRY_OPAQUE_BIT_KHR // indicates that this geometry does not invoke the any-hit shaders even if present in a hit group.
//			// VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR // indicates that the implementation must only call the any-hit shader a single time for each primitive in this geometry. If this bit is absent an implementation may invoke the any-hit shader more than once for this geometry.
//			;
//private:
//	VulkanMesh* _vulkanmesh = null;
public:
	explicit AbstractMesh();
	virtual ~AbstractMesh() override;
//public:
//	UpdateType updateType() const{ return _updateType; }
//	void setUpdateBackground();
public:
	bool isModified() const{ return _modificationState != NotModified; }
	bool isModifiedInGPU() const{ return _modificationState == ModifiedInGPU; }
	void setUnmodified();
	void setModified( ModificationState = ModifiedInCPU );
public:
	virtual uint32_t abstractVertexCount() const = 0;
	virtual uint32_t abstractVertexSize() const = 0;
	virtual uint32_t abstractVertexMemorySize() const { return abstractVertexCount() * abstractVertexSize(); }
	virtual const void* abstractVertexData() const = 0;
	virtual uint32_t abstractIndexCount() const = 0;
	virtual uint32_t abstractIndexMemorySize() const { return abstractIndexCount() * sizeof( uint32_t ); }
	virtual const void* abstractIndexData() const = 0;
//public:
//	VkGeometryFlagsKHR geometryFlags() const { return _geometryFlags; }
//	void setGeometryFlags( VkGeometryFlagsKHR geometryFlags );
//public:
//	bool hasVulkanMesh() const{ return _vulkanmesh != null; }
//	VulkanMesh* vulkanMesh() const { ASSERT( _vulkanmesh ); return _vulkanmesh; }
//	void setVulkanMesh( VulkanMesh* vulkanmesh );
//public:
//	virtual void collectPreLoad( List<AbstractMesh*>& meshes ) override;
//public:
//	virtual void load( BinaryFileReader& reader ) override;
//	virtual void save( BinaryFileWriter& writer ) const override;
};

template < typename V >
class Mesh : public AbstractMesh
{
	DEBUGCOUNTER(Mesh)
protected:
	std::vector<V> _vertices;
	std::vector<uint32_t> _indices;
public:
	explicit Mesh(){
	}
	explicit Mesh( int maxVertexCount, int maxIndexCount ){
		_vertices.reserve( maxVertexCount );
		_indices.reserve( maxIndexCount );
	}
	virtual ~Mesh() override {
	}
public:
    Box3 calcBoundingBox() const{
        Box3 box;
		calcBoundingBox( box );
		return box;
	}
    void calcBoundingBox( Box3& box ) const{
		box.reset();
		for( uint32_t v = 0; v < vertexCount(); v++ ){
			box.unite( _vertices[v].position );
		}
	}
public:
	bool isEmpty() const{
		if( 0 < vertexCount() )return false;
		return true;
	}
	void clear(){
		_vertices.resize( 0 );
		_indices.resize( 0 );
		setModified();
	}
public:
	uint32_t vertexCount() const {
		return _vertices.size();
	}
	const V& vertex( uint32_t v ) const {
		assert( v < vertexCount() );
		return _vertices.at( v );
	}
	V& vertex( uint32_t v ){
		assert( v < vertexCount() );
		return _vertices.at( v );
	}
	uint32_t addVertex( const V& vertex ){
		uint32_t i = _vertices.size();
		_vertices.push_back( vertex );
		setModified();
		return i;
	}
	const void* verticesData() const{ return _vertices.data(); }
public:
	uint32_t indexCount() const {
		return _indices.size();
	}
	void addIndex( uint32_t i ){
		_indices.push_back( i );
		setModified();
	}
	void addIndex( uint32_t i0, uint32_t i1, uint32_t i2 ){
		_indices.push_back( i0 );
		_indices.push_back( i1 );
		_indices.push_back( i2 );
		setModified();
	}
	void addIndex( uint32_t i00, uint32_t i10, uint32_t i11, uint32_t i01 ){
		addIndex( i00, i10, i11 );
		addIndex( i00, i11, i01 );
	}
	uint32_t index( uint32_t i ) const {
		assert( i < indexCount() );
		return _indices.at( i );
	}
	const void* indicesData() const{ return _indices.data(); }
public:
	void addMesh( const Mesh<V>& src ){
		uint32_t index_offset = vertexCount();
		for( uint32_t v = 0; v < src.vertexCount(); v++ ){
			addVertex( src.vertex( v ) );
		}
		for( uint32_t i = 0; i < src.indexCount(); i++ ){
			addIndex( index_offset + src.index( i ) );
		}
	}
	Mesh<V>* clone() const{
		Mesh<V>* target = new Mesh<V>( vertexCount(), indexCount() );
		for( uint32_t v = 0; v < vertexCount(); v++ ){
			target->addVertex( vertex( v ) );
		}
		for( uint32_t i = 0; i < indexCount(); i++ ){
			target->addIndex( index( i ) );
		}
		return target;
	}
public:
	void transform( const Mat4& matrix ){
		for( uint v = 0; v < _vertices.size(); v++ ){
			_vertices.at( v ).position = matrix.map( Vec3( _vertices.at( v ).position ) );
			_vertices.at( v ).normal = matrix.mapNormal( Vec3( _vertices.at( v ).normal ) );
		}
		setModified();
	}
//	void transformAbsoluteToRelative( const PosOri& absolutePosOri ){
//		transform( absolutePosOri.matrix().inverted() );
//	}
	void centralize(){
        Box3 bb;
		calcBoundingBox( bb );
		transform( Mat4::Translation( -bb.center() ) );
	}
	void swapTriangles(){
		for( int i0 = 0; i0 + 2 < indexCount(); i0 += 3 ){
			int i1 = index( i0 + 1 );
			int i2 = index( i0 + 2 );
			_indices[ i0 + 1 ] = i2;
			_indices[ i0 + 2 ] = i1;
		}
		setModified();
	}
public:
	virtual uint32_t abstractVertexCount() const override { return _vertices.size(); }
	virtual uint32_t abstractVertexSize() const override { return sizeof( V ); }
	virtual const void* abstractVertexData() const override { return verticesData(); }
	virtual uint32_t abstractIndexCount() const override { return _indices.size(); }
	virtual const void* abstractIndexData() const override { return indicesData(); }
//public:
//	virtual ResourceType* type() const{ return asserted( V::GetResourceType() ); }
//public:
//	virtual void load( BinaryFileReader& reader ) override {
//		AbstractMesh::load( reader );
//		reader.read_magicnumber( 19176 );
//		assert( _vertices.size() == 0 );
//		uint vertex_count = reader.read_uint32();
//		uint index_count = reader.read_uint32();
//		_vertices.resize( vertex_count );
//		_indices.resize( index_count );
//		reader.read_block( _vertices.data(), sizeof( V ) * vertex_count );
//		reader.read_block( _indices.data(), sizeof( uint32_t ) * index_count );
//		reader.read_magicnumber( 19177 );
//	}
//	virtual void save( BinaryFileWriter& writer ) const override {
//		AbstractMesh::save( writer );
//		writer.write_uint32( 19176 );
//		assert( 0 < vertexCount() ); assert( 0 < indexCount() );
//		writer.write_uint32( vertexCount() );
//		writer.write_uint32( indexCount() );
//		writer.write_block( _vertices.data(), sizeof( V ) * _vertices.size() );
//		writer.write_block( _indices.data(), sizeof( uint32_t ) * _indices.size() );
//		writer.write_uint32( 19177 );
//	}
public:
	friend class Resources;
	friend class AssImp;
};

//typedef Mesh<VertexPNT> MeshPNT;
#define MeshPNT Mesh<VertexPNT>

//MeshPNT* loadMeshPNT( BinaryFileReader& reader );
//void saveMeshPNT( BinaryFileWriter& writer, const MeshPNT& mesh );

//class MeshPNTType : public ResourceType
//{
//public:
//	static const char* Id;
//public:
//	explicit MeshPNTType();
//	virtual ~MeshPNTType() override;
//public:
//	virtual Resource* newInstance() override;
//};
