#pragma once

#include "Collection.h"

//class CollectionBuilder
//{
//private:
//	Collection* _collection;
//private:
//	ObjectType* _objecttype = null;
//public:
//	explicit CollectionBuilder( Collection* collection );
//	virtual ~CollectionBuilder();
//public:
//	void createMaterial( const String& id, std::function<void( Material& material, CollectionImporter& importer )> setter );
//	void importMaterial( const String& id, const String& filename );
//	void importMaterial( const String& id, const String& filename, std::function<void( Material& material, CollectionImporter& importer )> setter );
//public:
//	void startObject( const String& id );
//	void importMesh( const String& filename, uint meshindex, const String& materialid );
//	void importVegetationMesh( const String& filename, uint meshindex, const String& material0id, const String& material1id );
//	void addMesh( const String& meshid, const String& materialid );
//};

class CollectionImporter
{
	DEBUGCOUNTER( CollectionImporter )
public: // copy in AssImp.h
	static Mat4 YUp_to_ZUp_Synty1(){ return Mat4::Scale( 1.0f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward ); }
	static Mat4 YUp_to_ZUp_Synty2(){ return Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward ); }
private:
	Collection* _collection;
//private:
//	String _source_path;
public:
	explicit CollectionImporter( Collection* collection );
	virtual ~CollectionImporter();
public:
	Collection* collection(){ return _collection; }
//public:
//	void setSourcePath( const String& sourcepath );
//	String getSourceFilename( const String& filename );
public:
	void resource( const String& id, Resource* resource );
	Image* image( const String& id, const String& filename );
	Texture* texture( const String& id, const String& filename );
	Material* material( const String& id );
	Material* materialWithTexture( const String& id, const String& filename );
//	Material* importParticleMaterial( const String& id, const String& filename );
//	MeshModel* importModel( const String& filename );
	MeshPNT* mesh( const String& id, const String& filename, const Mat4& transform, uint meshindex = UINT32_MAX );
//	void importMesh( const String& id, MeshPNT* mesh );
//	void importMeshAndConvexHull( const String& id, MeshPNT* mesh );
//	void importSyntyVegetation2022Mesh( const String& id, const String& filename, uint meshindex );
//public:
};

//class CollectionImporterEntry {
//	DEBUGCOUNTER( CollectionImporterEntry )
//public:
//	explicit CollectionImporterEntry();
//	virtual ~CollectionImporterEntry();
//public:
//	virtual void import( CollectionImporter& importer ) = 0;
//};

//class CollectionImportCreateMaterial : public CollectionImporterEntry
//{
//private:
//	String _material_id;
//	String _texture_filename;
//	std::function<void( Material& material, CollectionImporter& importer )> _setter;
//public:
//	explicit CollectionImportCreateMaterial( const String& material_id, const String& texture_filename, std::function<void( Material& material, CollectionImporter& importer )> setter );
//	virtual ~CollectionImportCreateMaterial() override;
//public:
//	virtual void import( CollectionImporter& importer ) override;
//};

//class CollectionImportMesh : public CollectionImporterEntry
//{
//private:
//	String _meshid;
//	String _filename;
//	uint _meshindex;
//public:
//	explicit CollectionImportMesh( const String& meshid, const String& filename, uint meshindex );
//	virtual ~CollectionImportMesh() override;
//public:
//	virtual void import( CollectionImporter& importer ) override;
//};

//class CollectionImportVegetationMesh : public CollectionImporterEntry
//{
//private:
//	String _meshid;
//	String _filename;
//	uint _meshindex;
//public:
//	explicit CollectionImportVegetationMesh( const String& meshid, const String& filename, uint meshindex );
//	virtual ~CollectionImportVegetationMesh() override;
//public:
//	virtual void import( CollectionImporter& importer ) override;
//};
