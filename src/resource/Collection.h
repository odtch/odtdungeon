#pragma once

#include "ResourceCore.h"
#include "utils/Singleton.h"
#include "utils/LookupTable.h"
#include "Resource.h"
#include "Texture.h"
#include "Material.h"
#include "MeshModel.h"

class Collection : public Singleton
{
	DEBUGCOUNTER( Collection )
private:
	String _id;
private:
	bool _freezed = false;
private:
	List<Resource*> _resources;
	LookupTable<std::string,Resource*> _id2resource;
//private:
//	List<Texture*> _textures;
//	List<Material*> _materials;
////	List<MaterialAlternatives*> _materialAlternatives;
//	List<MeshModel*> _meshmodels;
//private:
//	List<ObjectType*> _objects;
//private:
//	List<CollectionImporterEntry*> _importers;
public:
	explicit Collection( const char* id );
	virtual ~Collection() override;
protected:
	void load( const String& filename );
	void save( const String& filename );
	virtual void import( CollectionImporter& importer ) = 0;
//	virtual void build();
//	virtual void createTests();
private:
//	void addResource( Resource* resource );
	void addResource( const String& id, Resource* resource );
public:
//	const List<Resource*>& resources() const{ return _resources; }
	Resource* getResource( const String& id ) const;
public:
//	const List<Texture*>& textures() const{ return _textures; }
	Texture* getTexture( const String& id ) const;
public:
//	const List<Material*>& materials() const{ return _materials; }
	Material* getMaterial( const String& id ) const;
public:
	MeshPNT* getMeshPNT( const String& id ) const;
//public:
//	const List<MeshModel*>& meshmodels() const{ return _meshmodels; }
//	MeshModel* getMeshModel( const String& id ) const;
//public:
//	PhysicsShape* getShape( const String& id ) const;
//public:
//	void addObject( ObjectType* object );
//	const List<ObjectType*>& objects() const{ return _objects; }
//	ObjectType* getObject( const String& id ) const;
//public:
//	void addImporter( CollectionImporterEntry* importer );
////protected: public:
////	Material* createMaterial( const String& materialName );
////	Material* loadMaterial( const String& filename );
////	Material* getMaterialAlternative( Material* orimaterial, const String& altname );
////	void setMaterialAlternative( Material* orimaterial, Material* alternative );
////	MeshModel* loadMeshModel( const String& filename );
////public:
////protected:
////	void addResourceToDelete( Resource* resource );
public:
	friend class Collections;
	friend class CollectionImporter;
};

class Collections : public Singleton
{
	DEBUGCOUNTER( Collections );
public:
	static const char* Id;
	static Collections* Get(){ return Singleton::Get<Collections>(); }
private:
	bool _freezed = false;
	List<Collection*> _collections;
public:
	explicit Collections();
	virtual ~Collections() override;
public:
	const List<Collection*>& collections() const{ return _collections; }
	Collection* find( const String& id ) const;
	Collection* get( const String& id ) const;
	void load( const String& mediapath );
private:
	friend class Collection;
};
