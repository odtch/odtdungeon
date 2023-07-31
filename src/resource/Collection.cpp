#include "stdodt.h"
#include "Collection.h"
#include "CollectionImporter.h"
#include "ResourceType.h"
#include "ResourceStorage.h"
//#include "OdtResource.h"
//#include "object/ObjectType.h"
//#include "physics/PhysicsShapes.h"

Collection::Collection( const char* id )
	:Singleton( id )
	,_id( id )
{
	ASSERT( !Collections::Get()->_freezed );
	Collections::Get()->_collections.add( this );
}
Collection::~Collection(){
//	_importers.deleteAll();
//	_meshmodels.removeAll();
//	_materials.removeAll();
////	_materialAlternatives.deleteAll();
//	_textures.removeAll();
	_id2resource.clear();
	_resources.deleteAll();
//	_objects.removeAll();
}
void Collection::load( const String& filename ){
	assert( !_freezed );
	assert( _resources.isEmpty() );
	ResourceReader reader( filename );
	reader.read_magicnumber( 724730 );
	uint32_t resourcecount = reader.read_uint32();
	while( _resources.size() < resourcecount ){
		reader.read_magicnumber( 724731 );
		String id;
		reader.read_string( id );
		reader.read_magicnumber( 724732 );
		Resource* resource = reader.read_resource();
		addResource( id, resource );
		reader.read_magicnumber( 724733 );
	}
	reader.read_magicnumber( 724734 );
//	for( ObjectType* object : _objects ){
//		object->load( *this );
//	}
}
void Collection::save( const String& filename ){
	ResourceWriter writer( filename );
	writer.write_uint32( 724730 );
	writer.write_uint32( _resources.size() );
//	_id2resource.foreachKeyValue( [&writer]( const std::string& id, Resource* resource ){
//		writer.write_uint32( 724731 );
//		writer.write_string( String( id ) );
//		writer.write_uint32( 724732 );
//		writer.write( resource );
//		writer.write_uint32( 724733 );
//	});
	for( Resource* resource : _resources ){
		writer.write_uint32( 724731 );
		writer.write_string( resource->id() );
		writer.write_uint32( 724732 );
		writer.write( resource );
		writer.write_uint32( 724733 );
	}
	writer.write_uint32( 724734 );
	writer.close();
}
//void Collection::import( CollectionImporter& importer ){
//	for( CollectionImporterEntry* importerentry : _importers ){
//		importerentry->import( importer );
//	}
//	for( ObjectType* object : _objects ){
//		object->import( importer );
//	}
//}
//void Collection::build(){
//	assert( !_freezed );
//	for( ObjectType* object : objects() ){
//		object->build( *this );
//	}
//	_freezed = true;
//}
//void Collection::createTests(){
//}
void Collection::addResource( const String& id, Resource* resource ){
	assert( !_freezed );
	assert( !id.isEmpty() );
	assert( resource );
	assert( resource->id().isEmpty() );
	resource->_id = id;
	assert( !_id2resource.contains( id.asStd() ));
	_resources.add( resource );
	_id2resource.add( id.asStd(), resource );
	//		Texture* texture = dynamic_cast<Texture*>( resource );
	//		if( texture ){
	//			_textures.add( texture );
	//		}
	//	}
	//	{
	//		Material* material = dynamic_cast<Material*>( resource );
	//		if( material ){
	//			_materials.add( material );
	//		}
	//	}
	//	{
	//		MeshModel* model = dynamic_cast<MeshModel*>( resource );
	//		if( model ){
	//			_meshmodels.add( model );
	//		}
}
Resource* Collection::getResource( const String& id ) const {
	assert( !id.isEmpty() );
	return _id2resource.get( id.asStd() );
}
Texture* Collection::getTexture( const String& id ) const{
	return asserted( dynamic_cast<Texture*>( getResource( id ) ) );
}
Material* Collection::getMaterial( const String& id ) const{
	return asserted( dynamic_cast<Material*>( getResource( id ) ) );
}
//* Collection::getMaterial( const String& id ) const {
//	Resource* resource = getResource( id );
//	Material* material = dynamic_cast<Material*>( resource );
//	assert( material );
//	return material;
//}
Mesh<VertexPNT>* Collection::getMeshPNT( const String& id ) const {
	return asserted( dynamic_cast<MeshPNT*>( getResource( id ) ) );
}
//MeshModel* Collection::getMeshModel( const String& id ) const {
//	Resource* resource = getResource( id );
//	MeshModel* meshmodel = dynamic_cast<MeshModel*>( resource );
//	assert( meshmodel );
//	return meshmodel;
//}
//PhysicsShape* Collection::getShape( const String& id ) const {
//	Resource* resource = getResource( id );
//	return asserted( dynamic_cast<PhysicsShape*>( resource ) );
//}
//void Collection::addObject( ObjectType* object ){
//	assert( object );
//	object->internalSetCollection( this );
//	_objects.add( object );
//}
//ObjectType* Collection::getObject( const String& id ) const{
//	for( ObjectType* object : _objects ){
//		if( object->id() == id )
//			return object;
//	}
//	assert( false );
//}
//void Collection::addImporter( CollectionImporterEntry* importer ){
//	_importers.add( importer );
//}
///*
//Material* Collection::getMaterial( const String& materialName ){
//	for( Material* material : _materials ){
//		if( material->id() == materialName )
//			return material;
//	}
//	for( Material* material : _materials ){
//		logDebug( material->id() );
//	}
//	logError( "material not found", materialName );
//	assert( false );
//}
//MeshModel* Collection::getMeshModel( const String& name ){
//	for( MeshModel* model : _meshmodels ){
//		if( model->id() == name )
//			return model;
//	}
//	for( MeshModel* model : _meshmodels ){
//		logDebug( model->id() );
//	}
//	logError( "model not found", name );
//	assert( false );
//}
//Material* Collection::createMaterial(const String& materialName){
//	Material* material = new Material();
//	material->setId( materialName );
//	_materials.add( material );
//	return material;
//}
//Material* Collection::loadMaterial( const String& filename ){
//	assert( false );
//	Texture* texture = new Texture( filename );
//	_textures.add( texture );
//	Material* material = new Material();
//	material->setTexture( texture );
//	_materials.add( material );
//	return material;

//}
//Material* Collection::getMaterialAlternative( Material* orimaterial, const String& altname ){
//	assert( orimaterial );
//	Material* altmaterial = getMaterial( altname );
//	setMaterialAlternative( orimaterial, altmaterial );
//	return altmaterial;
//}
//void Collection::setMaterialAlternative( Material* orimaterial, Material* altmaterial ){
//	MaterialAlternatives* alternatives = orimaterial->_alternatives;
//	if( alternatives == null ){
//		alternatives = new MaterialAlternatives();
//		_materialAlternatives.add( alternatives );
//		orimaterial->_alternatives = alternatives;
//		alternatives->_materials.add( orimaterial );
//	}
//	altmaterial->_alternatives = alternatives;
//	alternatives->_materials.add( altmaterial );
//}
//MeshModel* Collection::loadMeshModel( const String& filename ){
//	assert( false );
//	MeshModel* meshmodel = Resources::LoadMeshModel( filename, [this]( const String& materialName ){
//		return getMaterial( materialName );
//	} );
//	//meshmodel->setName( meshmodel );
//	_meshmodels.add( meshmodel );
//	return meshmodel;
//}

//void Collection::addResourceToDelete(Resource* resource)
//{
//int todo_rm;
//_resourcesToDelete.add( resource );
//}
//*/

const char* Collections::Id = "Collections";
Collections::Collections()
	:Singleton( Id )
{}
Collections::~Collections(){
	_collections.removeAll();
}
Collection* Collections::find( const String& id ) const {
	for( Collection* collection : _collections ){
		if( id == collection->id() ){
			return collection;
		}
	}
	return null;
}
Collection* Collections::get( const String& id ) const {
	return asserted( find( id ) );
}
void Collections::load( const String& mediapath ){
	_freezed = true;
	for( Collection* collection : _collections ){
		String filename = mediapath + "/" + collection->id() + ".obc";
		if( File::Exists( filename ) == false ){
			logDebug( "importing", collection->id() );
			CollectionImporter importer( collection );
			collection->import( importer );
			collection->save( filename );
		} else {
			collection->load( filename );
		}
	}
}
