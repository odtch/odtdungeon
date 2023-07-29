#include "stdodt.h"
#include "MeshModel.h"
#include "Material.h"
#include "utils/File.h"
//#include "ResourceStorage.h"

//const char* MeshModelType::Id = "MeshModelType";
//MeshModelType::MeshModelType()
//	:ResourceType( 4, Id )
//{
//}
//MeshModelType::~MeshModelType(){
//}
//Resource* MeshModelType::newInstance(){
//	return new MeshModel();
//}

MeshModel::MeshModel(){
}
MeshModel::~MeshModel(){
	_root_node = null;
	_allnodes.deleteAll();
	_meshes.deleteAll();
}
MeshModelNode* MeshModel::getNode( const String& name ) const {
	MeshModelNode* result = null;
	for( auto node : _allnodes ){
		if( node->name() == name ){
			assert( result == null );
			result = node;
		}
	}
	if( result == null ){
		for( auto node : _allnodes ){
			logDebug( node->name() );
		}
		logError( "MeshModel::getNode not found", name );
		assert( false );
	}
	return result;
}
MeshModelNode* MeshModel::createNode( const String& name, const Mat4& relativematrix, MeshModelNode* parent ){
	MeshModelNode* node = new MeshModelNode();
	_allnodes.add( node );
	node->_name = name;
	node->_parent = parent;
	node->_relative_matrix = relativematrix;
	if( _root_node == null ){
		assert( parent == null );
		_root_node = node;
	} else {
		assert( parent );
		parent->_childs.add( node );
	}
	return node;
}
Box3 MeshModel::calcBoundingBox() const {
	Box3 box;
	calcBoundingBox( box );
	return box;
}
void MeshModel::calcBoundingBox( Box3& box ) const{
	assert( box.isEmpty() );
	assert( _root_node );
	_root_node->calcBoundingBox( box );
}

void trace( MeshModel* model, int level, MeshModelNode* node ){
	//if( node->hasMesh() )
	{
		for( int l = 0; l <= level; l++ ){
			std::cout << "+- ";
		}
		if( node->meshes().isEmpty() ){
		} else {
			std::cout << "Mesh ";
		}
		std::cout << node->name() << " ";
		Vec3 pos = node->absoluteMatrix().map( Vec3::Null );
		if( pos.length() > 0 )
			std::cout << pos.toString();
//		if( node->hasMesh() ){
//			Box3D bb;
//			node->mesh()->calcBoundingBox( bb );
//			//std::cout << " " << bb.toString();
//			if( node->mesh()->indexCount() > 4800 )
//			std::cout << " " << node->mesh()->vertexCount() << " " << node->mesh()->indexCount();
//		}
		std::cout << "\n";
		for( MeshPNT* mesh : node->meshes() ){
			for( int l = 0; l <= level; l++ ){
				std::cout << "+- ";
			}
			//std::cout << "+- mesh " << model->meshes().indexOf( mesh->mesh() ) << " " << mesh->mesh()->id() << "\tmat " << ( mesh->hasMaterial() ? mesh->material()->id() : String() );
			std::cout << "+- mesh " << model->meshes().indexOf( mesh )
					  //<< " " << mesh->name()
						 // << "\tmat " << ( mesh->hasMaterial() ? mesh->material()->id() : String()
															 ;
			std::cout << "\n";

		}
	}
	for( auto c : node->childs() ){
		trace( model, level + 1, c );
	}
}
void MeshModel::trace(){
	::trace( this, 0, _root_node );
	int index = 0;
	for( MeshPNT* mesh : _meshes ){
		std::cout << "Mesh " << index
				  //<< "\t" << mesh->name()
				  << "\tvc=" << mesh->vertexCount() << " ic=" << mesh->indexCount() << " bb=" << mesh->calcBoundingBox().toString() << "\n";
		index++;
	}
}
void MeshModel::load( BinaryFileReader& reader ){
	Resource::load( reader );
	reader.read_magicnumber( 382719 );
	uint32_t meshcount = reader.read_uint32();
	assert( _meshes.isEmpty() );
	while( _meshes.size() < meshcount ){
		MeshPNT* mesh = new MeshPNT();
		mesh->load( reader );
		_meshes.add( mesh );
	}
	reader.read_magicnumber( 382720 );
	assert( _root_node == null );
	_root_node = loadNode( reader );
	reader.read_magicnumber( 382721 );
}
void MeshModel::save( BinaryFileWriter& writer ) const {
	Resource::save( writer );
	writer.write_uint32( 382719 );
	writer.write_uint32( _meshes.size() );
	for( MeshPNT* mesh : _meshes ){
		mesh->save( writer );
	}
	writer.write_uint32( 382720 );
	saveNode( _root_node, writer );
	writer.write_uint32( 382721 );
}
MeshModelNode* MeshModel::loadNode( BinaryFileReader& reader ){
	if( reader.read_uint8() != 'n' ){
		assert( false );
	}
	MeshModelNode* node = new MeshModelNode();
	_allnodes.add( node );
	reader.read_string( node->_name );
	//logDebug( "N", node->_name );
	reader.read_block( &node->_relative_matrix( 0, 0 ), sizeof( float ) * 4 * 4 );
	uint meshCount = reader.read_uint32();
	for( uint m = 0; m < meshCount; m++ ){
//		String materialName;
//		reader.read_string( materialName );
//		Material* material = null;
//		if( !materialName.isEmpty() ){
//			material = materials( materialName );
//		}
		uint mesh_index = reader.read_uint32();
		auto mesh = _meshes.get( mesh_index );
		node->_meshes.add( mesh ); // new MeshModelMatMesh( material, mesh ) );
	}
	uint child_count = reader.read_uint32();
	while( node->_childs.size() < child_count ){
		auto child = loadNode( reader );
		child->_parent = node;
		node->_childs.add( child );
	}
	if( reader.read_uint8() != 'N' ){
		assert( false );
	}
	return node;
}
void MeshModel::saveNode( MeshModelNode* node, BinaryFileWriter& writer ) const {
	assert( node );
	writer.write_uint8( 'n' );
	writer.write_string( node->_name );
	writer.write_block( &node->_relative_matrix( 0, 0 ), sizeof( float ) * 4 * 4 );
	writer.write_uint32( node->_meshes.size() );
	for( auto matmesh : node->_meshes ){
		//writer.write_string( matmesh->hasMaterial() ? matmesh->material()->name() : String() );
		writer.write_uint32( _meshes.indexOf( matmesh ) );
	}
	writer.write_uint32( node->_childs.size() );
	for( auto child : node->_childs ){
		saveNode( child, writer );
	}
	writer.write_uint8( 'N' );
}

MeshModelNode::MeshModelNode(){
}
MeshModelNode::~MeshModelNode(){
	_parent = null;
	_meshes.removeAll();
	_childs.removeAll();
}

void MeshModelNode::recalc_absolute_matrix() const
{
	ASSERT( !_absolute_matrix_valid );
	if( _parent == null ){
		_absolute_matrix = _relative_matrix;
	} else {
		_absolute_matrix = _parent->absoluteMatrix() * _relative_matrix;
	}
	_absolute_matrix_valid = true;
}
//void MeshModelNode::createMeshMat( Mesh<VertexPNT>* mesh, Material* material ){
//	_meshes.add( new MeshModelMatMesh( material, mesh ) );
//}
void MeshModelNode::calcBoundingBox( Box3& box ) const {
	for( auto matmesh : _meshes ){
		Box3 meshbox;
		MeshPNT* mesh = matmesh;//->mesh();
		mesh->calcBoundingBox( meshbox );
		box.unite( absoluteMatrix().map( meshbox.min() ) );
		box.unite( absoluteMatrix().map( meshbox.max() ) );
	}
	for( auto child : _childs ){
		child->calcBoundingBox( box );
	}
}


//MeshModelMatMesh::MeshModelMatMesh(Material* material, Mesh<VertexPNT>* mesh)
//	:_material( material )
//	,_mesh( mesh )
//{
//}
//MeshModelMatMesh::~MeshModelMatMesh(){
//}
