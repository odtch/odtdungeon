#pragma once

#include "ResourceCore.h"

class Resource
{
	DEBUGCOUNTER( Resource )
	ODTNOCOPY( Resource )
private:
	uint _debugno = 0;
	String _id;
public:
	explicit Resource();
	virtual ~Resource();
public:
	const String& id() const{ return _id; }
public:
	virtual ResourceType* type() const{ assert( false ); }
public:
	virtual void load( BinaryFileReader& reader );
	virtual void save( BinaryFileWriter& writer ) const;
//public:
//	virtual void collectPreLoad( List<AbstractMesh*>& meshes ){ OUNUSED( meshes ); }
public:
//	virtual bool isTexture() const{ return false; }
//public:
//	virtual bool isSceneDescription() const{ return false; }
//	virtual SceneDescription* asSceneDescription(){ assert( false ); }
//public:
//	virtual bool isObjectType() const{ return false; }
//	virtual ObjectType* asObjectType(){ assert( false ); }
public:
	friend class Collection;
};
