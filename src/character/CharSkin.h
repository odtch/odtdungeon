#pragma once

#include "CharCore.h"

class CharSkinPartType {
	DEBUGCOUNTER(CharSkinPartType)
private:
	uint32_t _jointindex;
	String _jointname;
private:
	MeshPNT* _mesh = null;
private:
	explicit CharSkinPartType( uint32_t jointindex, const String& jointname, MeshPNT* mesh );
	~CharSkinPartType();
public:
	uint32_t jointindex() const{ return _jointindex; }
	const String& jointname() const{ return _jointname; }
	MeshPNT* mesh() const{ return _mesh; }
public:
	friend class CharSkinType;
	friend class CharImporter;
};

class CharSkinType : public Resource
{
	DEBUGCOUNTER(CharSkinType)
private:
	List<CharSkinPartType*> _parts;
public:
	explicit CharSkinType( const String& id = String() );
	virtual ~CharSkinType() override;
//public:
//	virtual ResourceType* type() const override;
public:
	const List<CharSkinPartType*>& parts() const{ return _parts; }
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
public:
	friend class CharImporter;
};

class CharSkinPart : public ObjectRenderableProperty {
private:
	CharSkinPartType* _type = null;
private:
	CharJoint* _joint = null;
private:
	explicit CharSkinPart( CharSkinPartType* type, Material* material, CharJoint* joint, Object* object );
	~CharSkinPart();
public:
	const CharSkinPartType* type() const{ return _type; }
public:
	virtual void animate( float dt ) override;
public:
	friend class CharSkin;
	friend class CharImporter;
};

class CharSkin : public ObjectProperty
{
	DEBUGCOUNTER(CharSkin)
private:
	CharSkinType* _type;
	Material* _material;
	CharRagdoll* _skeleton;
private:
	List<CharSkinPart*> _parts;
public:
	explicit CharSkin( CharSkinType* type, Material* material, CharRagdoll* skeleton, Object* object );
	virtual ~CharSkin();
public:
};

