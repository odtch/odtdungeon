#include "stdodt.h"
#include "CollectionImporter.h"
#include "Collection.h"
#include "converter/AssImp.h"
//#include "MeshBuilder.h"
//#include "object/OdtObject.h"
//#include "physics/PhysicsShapes.h"

//CollectionBuilder::CollectionBuilder(Collection* collection)
//	:_collection( collection )
//{
//}
//CollectionBuilder::~CollectionBuilder(){
//}
//void CollectionBuilder::createMaterial( const String& id, std::function<void (Material&, CollectionImporter&)> setter ){
//	_collection->addImporter( new CollectionImportCreateMaterial( id, String(), setter ) );
//}
//void CollectionBuilder::importMaterial( const String& id, const String& texture_filename ){
//	_collection->addImporter( new CollectionImportCreateMaterial( id, texture_filename, []( Material& material, CollectionImporter& importer ){
//	} ) );
//}
//void CollectionBuilder::importMaterial( const String& id, const String& texture_filename, std::function<void (Material&, CollectionImporter&)> setter ){
//	_collection->addImporter( new CollectionImportCreateMaterial( id, texture_filename, setter ) );
//}
//void CollectionBuilder::startObject( const String& id ){
//	_objecttype = new ObjectType( id );
//	_collection->addObject( _objecttype );
//}
//void CollectionBuilder::importMesh( const String& filename, uint meshindex, const String& materialid ){
//	String meshid = File::GetNameFromPath( filename ) + "_" + String::FromInt( meshindex );
//	_collection->addImporter( new CollectionImportMesh( meshid, filename, meshindex ) );
//	assert( _objecttype );
//	new ObjectLazyRenderablePropertyType( meshid, materialid, _objecttype );
//}
//void CollectionBuilder::importVegetationMesh( const String& filename, uint meshindex, const String& material0id, const String& material1id ){
//	String meshid = File::GetNameFromPath( filename ) + "_" + String::FromInt( meshindex );
//	_collection->addImporter( new CollectionImportVegetationMesh( meshid, filename, meshindex ) );
//	assert( _objecttype );
//	new ObjectLazyRenderablePropertyType( meshid + "_0", material0id, _objecttype );
//	new ObjectLazyRenderablePropertyType( meshid + "_1", material1id, _objecttype );
//}
//void CollectionBuilder::addMesh(const String& meshid, const String& materialid){
//	assert( _objecttype );
//	new ObjectLazyRenderablePropertyType( meshid, materialid, _objecttype );
//}




CollectionImporter::CollectionImporter( Collection* collection )
	:_collection( asserted( collection ) )
{
}
CollectionImporter::~CollectionImporter(){
	_collection = null;
}
//void CollectionImporter::setSourcePath(const String& sourcepath){
//	_source_path = sourcepath;
//}
//String CollectionImporter::getSourceFilename( const String& filename ){
//	if( filename.startsWith( "/" ) )
//		return filename;
//	assert( !_source_path.isEmpty() );
//	return _source_path + "/" + filename;
//}
Image* CollectionImporter::image( const String& id, const String& filename ){
#ifdef ODTDEBUG
	Image* image = new Image();
	image->import( filename );
	_collection->addResource( id, image );
	return image;
#else
	assert( false );
#endif
}
Texture* CollectionImporter::texture( const String& id, const String& filename ){
	Image* image = this->image( id + "Image", filename );
	Texture* texture = new Texture();
//	texture->setId( id );
	texture->setImage( image );
	_collection->addResource( id, texture );
	return texture;
}
Material* CollectionImporter::material( const String& id ){
	Material* material = new Material();
//	material->setId( id );
	_collection->addResource( id, material );
	return material;
}
Material* CollectionImporter::materialWithTexture( const String& id, const String& filename ){
	Texture* texture = this->texture( id + "Texture", filename );
	Material* material = this->material( id );
	material->setTexture( texture );
	return material;
}
//Material* CollectionImporter::importMaterial( const String& id, const String& filename ){
//	Texture* texture = importTexture( id + "Texture", filename );
//	Material* material = createMaterial( id );
//	material->setTexture( texture );
//	return material;
//}
//Material* CollectionImporter::importParticleMaterial( const String& id, const String& filename ){
//	Material* material = importMaterial( id, filename );
//	material->setFlag( MaterialFlag_NoLightAffected );
//	material->setFlag( MaterialFlag_NoShadowEmit );
//	//material->setFlag( MaterialFlag_CalcNormalFromTriangle );
//	//material->setFlag( MaterialFlag_ViewOrentationToAlpha );
//	material->setTranslucent( true );
//	return material;
//}
//MeshModel* CollectionImporter::importModel( const String& filename ){
//	AssImp assimp;
//	Mat4 transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
//	//assimp._node_transform = assimp._mesh_transform;
//	assimp.open( getSourceFilename( filename ).asStd(), transform );
////	std::stringstream ss;
////	assimp.trace( ss );
////	String info = String( ss.str() );
//	//logDebug( filename, info );
//	MeshModel* model = assimp.loadModel();
//	_collection->addResource( model );
//	return model;
//}
Mesh<VertexPNT>* CollectionImporter::mesh( const String& id, const String& filename, const Mat4& transform, uint meshindex ){
	AssImp assimp;
	assimp.open( filename, transform );
	assimp.trace();
	if( meshindex == UINT32_MAX ){
		assert( assimp.meshCount() == 1 );
		meshindex = 0;
	}
	MeshPNT* mesh = assimp.loadMeshPNT( meshindex );
	_collection->addResource( id, mesh );
	return mesh;
}
//void CollectionImporter::importMesh( const String& id, Mesh<VertexPNT>* mesh ){
//	assert( mesh );
//	mesh->setId( id );
//	_collection->addResource( mesh );
//}
//void CollectionImporter::importMeshAndConvexHull( const String& id, Mesh<VertexPNT>* mesh ){
//	importMesh( id + "Mesh", mesh );
//	PhysicsShape* shape = PhysicsShape::CreateConvexHull( *mesh );
//	shape->setId( id + "Shape" );
//	_collection->addResource( shape );
//}
//void CollectionImporter::importSyntyVegetation2022Mesh(	const String& id, const String& filename, uint meshindex ){
//	AssImp assimp;
////	assimp._mesh_transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right );// * Mat4::RotationAtAxis( -90, Vec3::Up ) * Mat4::RotationAtAxis( -90, Vec3::Right );
////	assimp._node_pretransform = assimp._mesh_transform * Mat4::RotationAtAxis( -90, Vec3::Right );
////	assimp._node_posttransform = Mat4::RotationAtAxis( 90, Vec3::Up );
//	assimp.open( getSourceFilename( filename ).asStd() );
//	MeshPNT* fullmesh = assimp.loadMeshPNT( meshindex );
//	MeshPNT* mesh_leaf = MeshBuilder::FilteredByVertex<VertexPNT>( *fullmesh, [&assimp,meshindex]( int v, const VertexPNT& vertex ){
//		vec4 color = assimp.getVertexColor( meshindex, v );
//		return color[2] > 0.5f;
//	} );
//	assert( !mesh_leaf->isEmpty() );
//	mesh_leaf->setId( id + "_1" );
//	_collection->addResource( mesh_leaf );
//	MeshPNT* mesh_bark = MeshBuilder::FilteredByVertex<VertexPNT>( *fullmesh, [&assimp,meshindex]( int v, const VertexPNT& vertex ){
//		vec4 color = assimp.getVertexColor( meshindex, v );
//		return color[2] < 0.5f;
//	} );
//	mesh_bark->setId( id + "_0" );
//	assert( !mesh_bark->isEmpty() );
//	_collection->addResource( mesh_bark );
//	odelete( fullmesh );
//}

//CollectionImporterEntry::CollectionImporterEntry(){
//}
//CollectionImporterEntry::~CollectionImporterEntry(){
//}

//CollectionImportMesh::CollectionImportMesh( const String& meshid, const String& filename, uint meshindex )
//	:_meshid( meshid )
//	,_filename( filename )
//	,_meshindex( meshindex )
//{
//}
//CollectionImportMesh::~CollectionImportMesh(){
//}
//void CollectionImportMesh::import( CollectionImporter& importer ){
//	importer.importMesh( _meshid, _filename, _meshindex );
//}

//CollectionImportVegetationMesh::CollectionImportVegetationMesh( const String& meshid, const String& filename, uint meshindex )
//	:_meshid( meshid )
//	,_filename( filename )
//	,_meshindex( meshindex )
//{
//}
//CollectionImportVegetationMesh::~CollectionImportVegetationMesh(){
//}
//void CollectionImportVegetationMesh::import( CollectionImporter& importer ){
//	importer.importSyntyVegetation2022Mesh( _meshid, _filename, _meshindex );
//}


//CollectionImportCreateMaterial::CollectionImportCreateMaterial(const String& material_id, const String& texture_filename, std::function<void (Material&,CollectionImporter& importer)> setter)
//	:_material_id( material_id )
//	,_texture_filename( texture_filename )
//	,_setter( setter )
//{
//}
//CollectionImportCreateMaterial::~CollectionImportCreateMaterial(){
//}
//void CollectionImportCreateMaterial::import( CollectionImporter& importer ){
//	Texture* texture = null;
//	if( _texture_filename.isEmpty() == false ){
//		texture = importer.importTexture( _material_id + "_txt", _texture_filename );
//	}
//	Material* material = importer.createMaterial( _material_id );
//	if( texture )
//		material->setTexture( texture );
//	_setter( *material, importer );
//}
