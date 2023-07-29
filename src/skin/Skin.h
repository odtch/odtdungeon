#pragma once

#include "SkinCore.h"
#include "scene/SceneRenderInstanceProperty.h"

class Skin : public SceneRenderInstanceProperty
{
private:
	SkinType* _type;
	Material* _material;
private:
	bool _bones_modified = false;
	Mat4 _bone_matrix[ SkinMaxBoneCount ];
private:
	MeshPNT* _mesh = null;
public:
	explicit Skin( SkinType* type, Material* material, SceneObject* object );
	virtual ~Skin();
public:
	SkinType* type(){ return _type; }
	MeshPNT* mesh(){ return _mesh; }
	Material* material(){ return _material; }
public:
	Mat4& bone_matrix( int boneindex ){ ASSERT( 0 <= boneindex && boneindex < SkinMaxBoneCount ); return _bone_matrix[ boneindex ]; }
	void loadBones( const Skeleton& skeleton );
	bool isBonesModified() const{ return _bones_modified; }
	void setBonesModified();
protected:
	virtual RenderInstance* createInstance( Renderer& renderer, RenderLayer* layer ) override;
};

