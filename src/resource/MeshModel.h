#pragma once

#include "ResourceCore.h"
#include "utils/List.h"
#include "Resource.h"
//#include "ResourceType.h"
#include "Mesh.h"

//class MeshModelType : public ResourceType
//{
//public:
//	static const char* Id;
//public:
//	explicit MeshModelType();
//	virtual ~MeshModelType() override;
//public:
//	virtual Resource* newInstance() override;
//};


// Model
//   |          0/1
//   +---n Node +----+
//         +  |n     |
//         |  +------+
//         +n
//         Mesh


class MeshModel : public Resource
{
	DEBUGCOUNTER(MeshModel);
private:
	List<MeshPNT*> _meshes;
private:
	MeshModelNode* _root_node = null;
	List<MeshModelNode*> _allnodes;
public:
	explicit MeshModel( const String& name );
	virtual ~MeshModel() override;
public:
//	virtual ResourceType* type() const override { return Singleton::Get<MeshModelType>(); }
public:
	const List<MeshPNT*>& meshes() const{ return _meshes; }
public:
	MeshModelNode* rootNode() const{ ASSERT( _root_node ); return _root_node; }
	const List<MeshModelNode*>& allNodes() const{ return _allnodes; }
	MeshModelNode* getNode( const String& name ) const;
public:
	MeshModelNode* createNode( const String& name, const Mat4& relativematrix, MeshModelNode* parent );
public:
	Box3 calcBoundingBox() const;
	void calcBoundingBox( Box3& box ) const;
public:
	void trace();
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
private:
	MeshModelNode* loadNode( BinaryFileReader& reader );
	void saveNode( MeshModelNode* node, BinaryFileWriter& writer ) const;
public:
	friend class Resources;
	friend class AssImp;
};
//class MeshModelMatMesh {
//private:
//	Material* _material;
//	MeshPNT* _mesh;
//public:
//	explicit MeshModelMatMesh( Material* material, MeshPNT* mesh );
//	~MeshModelMatMesh();
//public:
//	bool hasMaterial() const{ return _material != null; }
//	Material* material() const{ ASSERT( _material ); return _material; }
//	void setMaterial( Material* material ){ _material = material; }
//	MeshPNT* mesh() const{ return _mesh; }
//};
class MeshModelNode {
	DEBUGCOUNTER( MeshModelNode )
private:
	String _name;
private:
	MeshModelNode* _parent = null;
	List<MeshModelNode*> _childs;
private:
	Mat4 _relative_matrix;
	mutable bool _absolute_matrix_valid = false;
	mutable Mat4 _absolute_matrix;
private:
//	List<MeshModelMatMesh*> _meshes;
	List<MeshPNT*> _meshes;
public:
	explicit MeshModelNode();
	virtual ~MeshModelNode();
public:
	const String& name() const{ return _name; }
public:
	bool hasParent() const{ return _parent != null; }
	MeshModelNode* parent() const{ ASSERT( _parent ); return _parent; }
public:
	const List<MeshModelNode*>& childs() const{ return _childs; }
public:
	const Mat4& relativeMatrix() const{ return _relative_matrix; }
	const Mat4& absoluteMatrix() const{ if( !_absolute_matrix_valid )recalc_absolute_matrix(); return _absolute_matrix; }
private:
	void recalc_absolute_matrix() const;
public:
	const List<MeshPNT*>& meshes() const{ return _meshes; }
public:
	void createMeshMat( MeshPNT* mesh, Material* material );
private:
	void calcBoundingBox( Box3& box ) const;
public:
	friend class MeshModel;
	friend class Resources;
	friend class AssImp;
};
