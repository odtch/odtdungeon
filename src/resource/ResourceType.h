#pragma once

#include "ResourceCore.h"
#include "utils/Singleton.h"
#include "utils/LookupTable.h"

class ResourceType : public Singleton
{
private:
	uint8_t _key;
public:
	explicit ResourceType( uint8_t key, const char* id );
	virtual ~ResourceType();
public:
	uint8_t id() const{ return _key; }
public:
	virtual Resource* newInstance() = 0;
};

class ResourceTypes : public Singleton
{
	DEBUGCOUNTER( ResourceTypes )
public:
	static const char* Id;
	static ResourceTypes* Get(){ return Singleton::Get<ResourceTypes>(); }
private:
	List<ResourceType*> _types;
	LookupTable<uint8_t,ResourceType*> _key2type;
public:
	explicit ResourceTypes();
	virtual ~ResourceTypes() override;
public:
	ResourceType* get( uint8_t key ) const;
	const List<ResourceType*>& types() const{ return _types; }
	void internalRegisterType( ResourceType* type );
	void registerDefaultTypes();
};

class ImageType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit ImageType();
	virtual ~ImageType() override;
public:
	virtual Resource* newInstance() override;
};

class TextureType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit TextureType();
	virtual ~TextureType() override;
public:
	virtual Resource* newInstance() override;
};

class MaterialType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit MaterialType();
	virtual ~MaterialType() override;
public:
	virtual Resource* newInstance() override;
};
