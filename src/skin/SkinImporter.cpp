#include "stdodt.h"
#include "SkinImporter.h"
#include "SkinMesh.h"
#include "converter/AssImp.h"
#include "converter/AssImpUtils.h"

SkinType* SkinImporter::Import( AssImp& assimp, uint meshindex ){
	assert( meshindex < assimp.meshCount() );
	aiMesh* aimesh = assimp.aimesh( meshindex );
	assert( aimesh->mVertices );
	assert( aimesh->mNormals );
	unsigned int pTextureCoordsIndex = 0;
	assert( aimesh->HasTextureCoords( pTextureCoordsIndex ) );
	assert( aimesh->mTextureCoords );
	aiVector3D* aitexturecoords = aimesh->mTextureCoords[ pTextureCoordsIndex ];
	assert( aimesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE );
	int vertexCount = aimesh->mNumVertices;
	int indexCount = assimp.calcIndexCountForMesh( aimesh );
	SkinMesh* mesh = new SkinMesh( vertexCount, indexCount );
	SkinVertex vertex;
	for( int v = 0; v < vertexCount; v++ ){
		aiVector3D& aiPos = aimesh->mVertices[v];
		aiVector3D& aiNorm = aimesh->mNormals[v];
		const aiVector3D& aitexturecord = aitexturecoords[ v ];
		assert( aitexturecord.z == 0.0f );
		Vec3 normal( aiNorm.x, aiNorm.y, aiNorm.z );
		if( normal.length2() < 0.001f ){
			logDebug( "AssImp::normal of vertex is null", v, normal.toString() );
			normal = Vec3::Up;
		}
		vertex.position = assimp.transform().map( Vec3( aiPos.x, aiPos.y, aiPos.z ) );
		vertex.normal = assimp.transform().mapNormal( normal );
		vertex.texcoord.x = aitexturecord.x;
		vertex.texcoord.y = 1.0f - aitexturecord.y;
		mesh->addVertex( vertex );
	}
	for( uint f = 0; f < aimesh->mNumFaces; f++ ){
		aiFace* aiface = &aimesh->mFaces[f];
		switch( aiface->mNumIndices ){
		case 3:
			mesh->addIndex( aiface->mIndices[ 0 ] );
			mesh->addIndex( aiface->mIndices[ 1 ] );
			mesh->addIndex( aiface->mIndices[ 2 ] );
			break;
		default:
			assert( false );
		}
	}
	Skeleton* skeleton = assimp.loadSkeleton();
	SkinType* skin = new SkinType();
	skin->_mesh = mesh;
	assert( aimesh->mNumBones <= SkinMaxBoneCount );
	for( unsigned int b = 0; b < aimesh->mNumBones; b++ ){
		aiBone* aibone = aimesh->mBones[b];
		SkinBoneType* bone = new SkinBoneType();
		bone->index = skin->_bones.size();
		skin->_bones.add( bone );
		assert( bone->index == b );
		bone->name = String( aibone->mName.C_Str() );
		if( aibone->mNode == null ){
			assert( false );
		} else {
			bone->jointname = aibone->mNode->mName.C_Str();
		}
//		bone->mesh2bone = assimp.transform() * PosOri( Mat4( ai_mat4_cast( aibone->mOffsetMatrix ) ) ).matrix();
//		bone->mesh2bone = assimp.transform() * Mat4( ai_mat4_cast( aibone->mOffsetMatrix );
		SkeletonJoint* joint = skeleton->getJointByName( bone->jointname );
		bone->bone2mesh = joint->absolutematrix().inverted();
		for( uint w = 0; w < aibone->mNumWeights; w++ ){
			aiVertexWeight* weight = &aibone->mWeights[ w ];
			SkinVertex* vertex = &mesh->vertex( weight->mVertexId );
			vertex->addBoneWeight( bone->index, weight->mWeight );
		}
	}
	odelete( skeleton );
	for( int v = 0; v < mesh->vertexCount(); v++ ){
		SkinVertex* vertex = &mesh->vertex( v );
		assert( 0 <= vertex->bone_indexes[ 0 ] );
		assert( 0 < vertex->bone_weights[ 0 ] );
		vertex->adjustBoneWeightsTo1();
	}
	return skin;
}
