#pragma once

#include "AssImpCore.h"

class AssImp
{
public:
	static Mat4 YUp_to_ZUp(){ return Mat4::Scale( 1.0f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward ); }
	//static Mat4 YUp_to_ZUp(){ return Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward ); }
private:
	String _filename;
	String _modelname;
	Assimp::Importer* _aiImporter;
	const aiScene* _aiScene;
private:
	Mat4 _transform;
public:
	explicit AssImp();
	~AssImp();
public:
	void open( const String& filename, const Mat4& transform = YUp_to_ZUp() );
	void close();
public:
	const Mat4& transform() const{ return _transform; }
public:
	void trace();
	void trace( std::ostream& stream );
	void traceNode( std::ostream& stream, const std::string& level, aiNode* node, const glm::mat4& parentMatrix );
public:
	const aiScene* aiscene() const{ return asserted( _aiScene ); }
	MeshModel* loadModel();
	MeshModelNode* loadModelNode( aiNode* ainode, MeshModelNode* parent, MeshModel* model );
	uint meshCount() const;
    aiMesh* aimesh( uint meshindex );
	int calcIndexCountForMesh( const aiMesh* aimesh );
    MeshPNT* loadMeshPNT( uint meshindex );
	glm::vec4 getVertexColor( uint meshindex, uint vertexindex );
public:
	Skeleton* loadSkeleton();
	void loadSkeletonNode( Skeleton* skeleton, aiNode* node, SkeletonJoint* parent );
public:
	uint animationCount() const;
	AssImpAnimation* loadAnimation();

	glm::quat aiAnimGetRotationAt( aiNodeAnim* aichannel, double time );
	Vec3 aiAnimGetPositionAt( aiNodeAnim* aichannel, double time );


/*
	void loadMeshPNT( List<Mesh<VertexPNT>*>& meshes );
	void loadNodes( std::function<void( uint meshIndex, const glm::mat4& localMatrix, const glm::mat4& globalMatrix )> loader );
	void loadNodes( std::function<void( uint meshIndex, const glm::mat4& localMatrix, const glm::mat4& globalMatrix )> loader, const glm::mat4& parentMatrix, aiNode* node );
	SceneDescription* loadSceneDescription( const List<Mesh<VertexPNT>*>& meshes );
	SceneDescriptionNode* loadSceneDesciptionNode( aiNode* ainode, SceneDescriptionNode* parent, const List<Mesh<VertexPNT>*>& meshes );
	Skin* loadSkin();
	SkinSkeleton* loadSkinSkeleton();
	SkinJoint* loadSkinJoint( aiNode* node );
	Mesh<SkinVertex>* loadSkinMesh( uint mesh_index, SkinSkeleton* skeleton );
public:
	SkinAnimation* loadAnimation( uint animationIndex );
	*/
private:
	String fixName( const aiString& src ) const;
};
