#pragma once

#include "CharCore.h"

class CharRagdollType : public Resource
{
	DEBUGCOUNTER(CharRagdollType)
private:
	CharJointType* _root = null;
	List<CharJointType*> _joints;
private:
	SkinType* _skin = null;
public:
	struct JointToSkin {
		CharJointType* joint = null;
		Mat4 relativematrix;
	};
private:
	List<JointToSkin*> _joint2skins;
public:
	explicit CharRagdollType( const String& id = String() );
	virtual ~CharRagdollType() override;
public:
//	virtual ResourceType* type() const override;
public:
	CharJointType* root() const{ return _root; }
	const List<CharJointType*>& joints() const{ return _joints; }
	CharJointType* findJointByName( const String& name ) const;
	CharJointType* getJointByName( const String& name ) const;
private:
	CharJointType* createJoint( CharJointType* parent );
//	CharJointType* createJoint( const String& name, const Mat4& absoluteMatrix, CharJointType* parent );
public:
	SkinType* skin() const{ ASSERT( _skin ); return _skin; }
	const List<JointToSkin*>& joint2skins() const{ return _joint2skins; }
public:
	virtual void collectPreLoad( List<AbstractMesh*>& meshes );
public:
	virtual void load( BinaryFileReader& reader ) override;
	virtual void save( BinaryFileWriter& writer ) const override;
public:
	void trace() const;
private:
	friend class CharImporter;
};

class CharRagdoll
{
	DEBUGCOUNTER(CharRagdoll)
private:
	CharRagdollType* _type;
private:
	CharJoint* _root = null;
	List<CharJoint*> _joints;
public:
	explicit CharRagdoll( CharRagdollType* type );
	virtual ~CharRagdoll();
public:
	CharRagdollType* type() const{ return _type; }
public:
	CharJoint* root(){ return _root; }
	const List<CharJoint*>& joints() const{ return _joints; }
	CharJoint* getJoint( CharJointType* jointtype ) const;
	CharJoint* findJointByName( const String& name ) const;
	CharJoint* getJointByName( const String& name ) const;
public:
//	void setRootPosOri( const PosOri& rootposori );
	void loadPose( const CharPose& pose );
	void savePose( CharPose& pose ) const;
//	void load( const CharRagdoll& ragdoll );
};

class CharRagdollRenderer : public ObjectProperty
{
private:
	CharRagdoll* _ragdoll;
	Material* _material;
	PosOri _posori;
private:
	struct Renderer {
		CharJoint* _joint = null;
		ObjectRenderableProperty* _renderer = null;
	};
	List<Renderer*> _renderers;
public:
	explicit CharRagdollRenderer( CharRagdoll* ragdoll, Material* material, Object* object );
	virtual ~CharRagdollRenderer() override;
public:
	void setPosOri( const PosOri& posori );
public:
	virtual void animate( float dt ) override;
private:
	void rebuild();
};
/*
class CharRagdollSkinRenderer : public ObjectProperty
{
private:
	CharRagdoll* _ragdoll;
private:
	PosOri _posori;
private:
	Skin* _skin;
	SkinRenderer* _renderer = null;
public:
	explicit CharRagdollSkinRenderer( CharRagdoll* ragdoll, Material* material, Object* object );
	virtual ~CharRagdollSkinRenderer() override;
public:
	void setPosOri( const PosOri& posori );
public:
	virtual void animate( float dt ) override;
};

class CharRagdollSkeletonRenderer : public ObjectRenderableProperty
{
private:
	CharRagdoll* _ragdoll;
	MeshPNT _mesh;
	Material _material;
public:
	explicit CharRagdollSkeletonRenderer( CharRagdoll* ragdoll, Object* object );
	virtual ~CharRagdollSkeletonRenderer() override;
public:
	virtual void animate( float dt ) override;
private:
	void rebuild();
};
*/
