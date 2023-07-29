#include "stdodt.h"
#include "ResourceType.h"

ResourceType::ResourceType( uint8_t key, const char* id )
	:Singleton( id )
	,_key( key )
{
	ResourceTypes::Get()->internalRegisterType( this );
}
ResourceType::~ResourceType(){
}

const char* ResourceTypes::Id = "ResourceTypes";
ResourceTypes::ResourceTypes()
	:Singleton( Id )
{
	registerDefaultTypes();
}
ResourceTypes::~ResourceTypes(){
	_types.removeAll();
}
ResourceType* ResourceTypes::get( uint8_t id ) const {
	return _key2type.get( id );
}
void ResourceTypes::internalRegisterType( ResourceType* type ){
	assert( type );
	_key2type.add( type->id(), type );
	_types.add( type );
}
void ResourceTypes::registerDefaultTypes(){
	new ImageType();
	new TextureType();
	new MaterialType();
//	new MeshPNTType();
//	new MeshModelType();
}

#include "Image.h"
const char* ImageType::Id = "ImageType";
ImageType::ImageType()
	:ResourceType( 1, Id )
{
}
ImageType::~ImageType(){
}
Resource* ImageType::newInstance(){
	return new Image();
}

#include "Texture.h"
const char* TextureType::Id = "TextureType";
TextureType::TextureType()
	:ResourceType( 2, Id )
{
}
TextureType::~TextureType(){
}
Resource* TextureType::newInstance(){
	return new Texture();
}

#include "Material.h"
const char* MaterialType::Id = "MaterialType";
MaterialType::MaterialType()
	:ResourceType( 3, Id )
{
}
MaterialType::~MaterialType(){
}
Resource* MaterialType::newInstance(){
	return new Material();
}

