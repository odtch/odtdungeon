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
	explicit CharRagdollType();
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

class CharRagdoll : public SceneProperty
{
	DEBUGCOUNTER(CharRagdoll)	
private:
	CharRagdollType* _type;
private:
	CharJoint* _root = null;
	List<CharJoint*> _joints;
public:
	explicit CharRagdoll( CharRagdollType* type, SceneObject* object );
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

class CharRagdollSkin : public Skin
{
private:
	CharRagdoll* _ragdoll;
public:
	explicit CharRagdollSkin( CharRagdoll* ragdoll, Material* material, SceneObject* object );
	virtual ~CharRagdollSkin() override;
protected:
	virtual void animate( float dt ) override;
};
