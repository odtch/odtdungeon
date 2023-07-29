#include "stdodt.h"
#include "AssImp.h"
#include "AssImpUtils.h"
#include "AssImpAnimation.h"
#include "resource/Skeleton.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <filesystem>

int AssImp::calcIndexCountForMesh( const aiMesh* aimesh ){
	int indexCount = 0;
	for( unsigned int f = 0; f < aimesh->mNumFaces; f++ ){
		aiFace* aiface = &aimesh->mFaces[f];
		indexCount += aiface->mNumIndices;
	}
	return indexCount;
}
glm::mat3 aiMatrix3x3ToGlm( const aiMatrix3x3& from ){
	// https://lechior.blogspot.com/2017/05/skeletal-animation-using-assimp-opengl.html
	glm::mat3 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
	return to;
}
glm::mat4 aiMatrix4x4ToGlm( const aiMatrix4x4 &from ){
	// https://lechior.blogspot.com/2017/05/skeletal-animation-using-assimp-opengl.html
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}
glm::mat4 aiMatrix4x4ToMat4(const aiMatrix4x4 &matrix){
//	return glm::mat4( matrix.a1, matrix.a2, matrix.a3, matrix.a4,
//					  matrix.b1, matrix.b2, matrix.b3, matrix.b4,
//					  matrix.c1, matrix.c2, matrix.c3, matrix.c4,
//					  matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	return aiMatrix4x4ToGlm( matrix );
}
vec3 aiTovec3( const aiVector3D& aivector ){
	return vec3( aivector.x, aivector.y, aivector.z );
}
vec4 aiToQVector4D( const aiColor4D& aivector ){
	return vec4( aivector.r, aivector.g, aivector.b, aivector.a );
}

AssImp::AssImp()
	:_aiImporter( null )
	,_aiScene( null )
{
//	Mat4 r1, r2, s;
//	//r1.setRotationAtAxis( 180, Vec3::Right );
////	r2.setRotationAtAxis( -90, Vec3::Up );
//	s.setScale( 0.01f );
//	_transform = r2 * r1 * s;
//	//r.setRotationAtAxis( -90, Vec3::Up );
}
AssImp::~AssImp(){
	close();
	ASSERT( _aiImporter == null );
	ASSERT( _aiScene == null );
}
void AssImp::open( const String& filename, const Mat4& transform ){
	logDebug( "AssImp::open", filename );
	_transform = transform;
	assert( _aiImporter == null );
	_aiImporter = new Assimp::Importer();
	assert( _aiScene == null );
	_modelname = std::filesystem::path( filename.asStd() ).stem().u8string();
	if( String( filename ).contains( "mocap/" ) ){
		_aiImporter->SetPropertyBool( AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false ); // liefert bei synty falsches skeleton
	}
	_aiScene = _aiImporter->ReadFile( filename.asStd(), aiProcess_Triangulate | aiProcess_GenSmoothNormals /* | aiProcess_FlipUVs */
									  //| aiProcess_ValidateDataStructure -> crashes scifi-world-char
									  //| aiProcess_LimitBoneWeights -> crashes scifi-world-char
									  | aiProcess_PopulateArmatureData
									  | aiProcess_FindInvalidData
									  );
	if( _aiScene == null ){
		logError( _aiImporter->GetErrorString() );
		othrow( "RnAssImp feadFile failed" );
	}
	_filename = filename;
}
void AssImp::close(){
	if( _aiImporter ){
		logDebug( "AssImp.close", _filename );
		if( String( _filename ).contains( String( "Polygon_SciFi_Worlds" ) )
				&& String( _filename ).contains( String( "Characters" ) ) ){
			logDebug( "Assimp::close skipping delete aiImporter because Synty SciFiWorld characters crashes" );
			_aiImporter = null;
		}
	}
	odelete( _aiImporter );
	_aiScene = null;
	_filename.clear();
}
void AssImp::trace(){
	trace( std::cout );
	std::cout.flush();
}
void AssImp::trace( std::ostream& stream ){
	stream << "AssImp " << _filename << "\n";
	stream << "+- RootNode " << "\n";
	traceNode( stream, "   ", _aiScene->mRootNode, glm::mat4( 1.0f ) );
	stream << "+- Textures " << _aiScene->mNumTextures << "\n";
	for( unsigned int t = 0; t < _aiScene->mNumTextures; t++ ){
		aiTexture* texture = _aiScene->mTextures[t];
		stream << "  " << texture->mFilename.C_Str() << " " << texture->mWidth << "x" << texture->mHeight << " fh:" << texture->achFormatHint << "\n";//->mFilename.C_Str();
	}
	stream << "+- Materials " << _aiScene->mNumMaterials << "\n";
	for( unsigned int m = 0; m < _aiScene->mNumMaterials; m++ ){
		aiMaterial* aimaterial = _aiScene->mMaterials[m];
		aiString name;
		aimaterial->Get( AI_MATKEY_NAME, name );
		stream << "  " << m << " " << name.C_Str() << "\n";
		//AI_MATKEY_COLOR_AMBIENT
		for( int tt = aiTextureType_NONE; tt <= AI_TEXTURE_TYPE_MAX; tt++ ){
			aiTextureType textureType = (aiTextureType)tt;
			for( unsigned int t = 0; t < aimaterial->GetTextureCount(textureType); t++ ){
				aiString textureName;
				aiTextureMapping textureMapping;
				aimaterial->GetTexture( textureType, t, &textureName, &textureMapping );
				stream << "    t " << tt << " " << t << textureName.C_Str() << "\n";
				ASSERT( textureMapping == aiTextureMapping_UV );
			}
		}
		for( uint p = 0; p < aimaterial->mNumProperties; p++ ){
			const aiMaterialProperty* aimatprop = aimaterial->mProperties[p];
			stream << "    p " << p << " " << aimatprop->mKey.C_Str();
			if( aimatprop->mKey.length < 14 )
				stream << "\t";
			stream << "\tsemantic=" << aimatprop->mSemantic << "\t";
			switch( aimatprop->mType ){
			case aiPTI_Float:
				stream << "\t";
				{	float* data = (float*) aimatprop->mData;
					int count = aimatprop->mDataLength / sizeof( float );
					assert( count * sizeof( float ) == aimatprop->mDataLength );
					stream << "( ";
					for( int i = 0; i < count; i++ ){
						if( i == 0 ){
						} else {
							stream << ", ";
						}
						stream << data[i] << "f";
					}
				}
				stream << " )";


				break;
			case aiPTI_Double:
				stream << "double-array";
				break;
			case aiPTI_String:
				stream << "string";
				break;
			case aiPTI_Integer:
				stream << "integer";
				break;
			case aiPTI_Buffer:
				stream << "buffer";
				break;
			default:
				stream << "(unhandled type)";
			}
			stream << "\n";
		}
	}
	stream << "+- Mehshes " << _aiScene->mNumMeshes << "\n";
	for( unsigned int m = 0; m < _aiScene->mNumMeshes; m++ ){
		aiMesh* aimesh = _aiScene->mMeshes[m];
		stream << "  " << m << " " << aimesh->mName.C_Str() << "\t";
		stream << "\n     "
			<< " primitiveType " << aimesh->mPrimitiveTypes
			<< " numVertices " << aimesh->mNumVertices
			<< " numFaces " << aimesh->mNumFaces
			<< " materialIndex " << aimesh->mMaterialIndex
			<< " method " << aimesh->mMethod
			<< " normals=" << ( aimesh->mNormals == null ? "no" : "yes" )
			<< " tangents=" << ( aimesh->mTangents == null ? "no" : "yes" )
			<< " bitangents=" << ( aimesh->mBitangents == null ? "no" : "yes" )
			//<< " colors=" << ( aimesh->mColors == null ? "no" : "yes" )
			//<< " textureCoords " << ( aimesh->mTextureCoords == null ? "no" : "yes" )
			<< " faces=" << ( aimesh->mFaces == null ? "no" : "yes" )
			<< " animMeshes=" << ( aimesh->mAnimMeshes == null ? "no" : "yes" )
			<< " numAnimMeshes=" << aimesh->mNumAnimMeshes
			<< " numBones=" << aimesh->mNumBones
			<< "\n";
		for( int j = 0; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++ ){
			if( aimesh->HasVertexColors( j ) ){
				aiColor4D* colors = aimesh->mColors[ j ];
				stream << "      cs " << j << " " << colors << "\n";
			}
		}
		for( int t = 0; t < AI_MAX_NUMBER_OF_TEXTURECOORDS; t++ ){
			if( aimesh->HasTextureCoords( t ) ){
				stream << "      tc " << t << "\n";
				//stream << "      " << t << aimesh->mTextureCoords[t];
				//for( int ti = 0; ti < 10; ti++ ){
					//stream << "        " << ti << aiTovec3( aimesh->mTextureCoords[t][ti] );
				//}
			}
		}
		//stream << "    mNumUVComponents " << aimesh->mNumUVComponents << "\n";
		for( unsigned int b = 0; b < aimesh->mNumBones; b++ ){
			aiBone* aibone = aimesh->mBones[b];
			stream << "      bone " << b << aibone->mName.C_Str() << " armature=";
			if( aibone->mArmature == null ){
				stream << "null";
			} else {
				stream << aibone->mArmature->mName.C_Str();
			}
			stream << " node=";
			if( aibone->mNode == null ){
				stream << "null";
			} else {
				stream << aibone->mNode->mName.C_Str();
			}
			stream << " numweights=" << aibone->mNumWeights << "\n";
		}
	}
	stream << "+- Animations " << _aiScene->mNumAnimations << "\n";
	for( unsigned int a = 0; a < _aiScene->mNumAnimations; a++ ){
		aiAnimation* aianimation = _aiScene->mAnimations[a];
		stream << "  " << a << " " << aianimation->mName.C_Str() << " duration=" << aianimation->mDuration << ", ticks per second=" << aianimation->mTicksPerSecond << "\n";
		stream << "  num channels=" << aianimation->mNumChannels << "\n";
		for( unsigned int c = 0; c < aianimation->mNumChannels; c++ ){
			aiNodeAnim* channel = aianimation->mChannels[c];
			stream << "  +- " << c << " " << channel->mNodeName.C_Str() << "\n";
			stream << "     number keys (pos,rot,scal)" << channel->mNumPositionKeys << ", " << channel->mNumRotationKeys << ", " << channel->mNumScalingKeys << "\n";
			/** Defines how the animation behaves before the first
			 *  key is encountered.
			 *
			 *  The default value is aiAnimBehaviour_DEFAULT (the original
			 *  transformation matrix of the affected node is used).*/
			stream << "     preState=" << channel->mPreState << "\n"; //C_ENUM aiAnimBehaviour mPreState << "\n";
			ASSERT( channel->mPreState == 0 );
			/** Defines how the animation behaves after the last
			 *  key was processed.
			 *
			 *  The default value is aiAnimBehaviour_DEFAULT (the original
			 *  transformation matrix of the affected node is taken).*/
			stream << "     postState=" << channel->mPostState << "\n"; //C_ENUM aiAnimBehaviour mPostState;
			ASSERT( channel->mPostState == 0 );

		}
		/** The node animation channels. Each channel affects a single node.
		 *  The array is mNumChannels in size. */
		//C_STRUCT aiNodeAnim** mChannels;
		stream << "  num mesh channels=" << aianimation->mNumMeshChannels << "\n";
		ASSERT( aianimation->mNumMeshChannels == 0 );
		/** The mesh animation channels. Each channel affects a single mesh.
		 *  The array is mNumMeshChannels in size. */
		//C_STRUCT aiMeshAnim** mMeshChannels;
		stream << "  num morph mesh channels=" << aianimation->mNumMorphMeshChannels << "\n";
		ASSERT( aianimation->mNumMorphMeshChannels == 0 );
		/** The morph mesh animation channels. Each channel affects a single mesh.
		 *  The array is mNumMorphMeshChannels in size. */
		//C_STRUCT aiMeshMorphAnim **mMorphMeshChannels;

	}
	stream.flush();
}
void traceMatrix( std::ostream& stream, const glm::mat4 matrix ){
	const vec3 translate( matrix[3] );
	const glm::quat rot( glm::toQuat( matrix ) );
	glm::vec3 euler = glm::eulerAngles( rot ) / 3.14159f * 180.f;
	vec3 sx( matrix[0][0], matrix[1][0], matrix[2][0] );
	vec3 sy( matrix[0][1], matrix[1][1], matrix[2][1] );
	vec3 sz( matrix[0][2], matrix[1][2], matrix[2][2] );
	vec3 scaling( glm::length( sx ), glm::length( sy ), glm::length( sz ) );
	stream << " tran=" << glm::to_string( translate )
			  << "\trot="
<< glm::to_string( euler )
				//<< glm::to_string( rot )
//				<< "\tscale=" << glm::to_string( scaling )
	;
}
void AssImp::traceNode( std::ostream& stream, const std::string &level, aiNode* node, const glm::mat4& parentMatrix ){
	glm::mat4 localMatrix = aiMatrix4x4ToMat4( node->mTransformation );
	glm::mat4 globalMatrix = parentMatrix * localMatrix;
	stream << level << "+- " << node->mName.C_Str();
	for( int s = level.length() + 3 + node->mName.length; s < 50; s++ ){
		stream << " ";
	}
	if( node->mNumMeshes == 0 )stream << " NOmesh";
	if( node->mNumMeshes > 1 )stream << " multi-mesh";
	for( uint m = 0; m < node->mNumMeshes; m++ ){
		stream << " mesh=" << node->mMeshes[m];
	}
	stream << "\t";
	stream << "local ";
	traceMatrix( stream, localMatrix );
	stream << "\t";
	stream << "abs ";
	traceMatrix( stream, globalMatrix );
/*
	stream << "\n       globl ";
	traceMatrix( globalMatrix );
	stream
//				<< " translate " << glm::to_string( localMatrix * vec4( 0, 0, 0, 1 ) )
//				<< " translate " << glm::to_string( translate  )
//				<< " rot " << glm::to_string( rot  )
//				<< " scale "
//					<< ( scaling.x == 1.0f && scaling.y == 1.0f && scaling.z == 1.0f ?
//							 "no" : glm::to_string( scaling ) )
				<< "\n";
	if( node->mMetaData ){
		stream << "      meta";
		for( uint p = 0; p < node->mMetaData->mNumProperties; p++ ){
			aiString* key = &node->mMetaData->mKeys[p];
			aiMetadataEntry* entry = &node->mMetaData->mValues[p];
			if( key != null )
				stream << " " << key->C_Str();
			if( entry != null ){
				stream << "=";
				switch( entry->mType ){
				case AI_BOOL:
					stream << "(bool)";
					break;
				case AI_INT32:
					stream << "(int32)";
					break;
				case AI_UINT64:
					stream << "(uint64)";
					break;
				case AI_FLOAT:
					stream << "(float)";
					break;
				case AI_DOUBLE:
					stream << "(double)";
					break;
				case AI_AISTRING:
					stream << "(str)";
					break;
				case AI_AIVECTOR3D:
					stream << "(vec3)";
					break;
				default:
					stream << "(unhandled type)";
				}
			}
		}
		stream << "\n";
	}
*/


	stream << "\n";
	stream.flush();
	std::string sublevel = level + "| ";
	for( unsigned int c = 0; c < node->mNumChildren; c++ ){
		aiNode* child = node->mChildren[c];
		traceNode( stream, sublevel, child, globalMatrix );
	}
}

MeshModel* AssImp::loadModel(){
	assert( false );
	/*
	MeshModel* model = new MeshModel();
	model->setId( _modelname );
	for( uint m = 0; m < _aiScene->mNumMeshes; m++ ){
		MeshPNT* mesh = loadMeshPNT( m );
		model->_meshes.add( mesh );
	}
	loadModelNode( _aiScene->mRootNode, null, model );
	for( MeshModelNode* node : model->allNodes() ){
		node->_absolute_matrix = node->absoluteMatrix();
	}
	for( MeshModelNode* node : model->allNodes() ){
		node->_absolute_matrix = _node_pretransform * node->_absolute_matrix * _node_posttransform;
	}
	for( MeshModelNode* node : model->allNodes() ){
		if( node->hasParent() == false ){
			node->_relative_matrix = node->_absolute_matrix;
		} else {
			node->_relative_matrix = node->parent()->_absolute_matrix.inverted() * node->_absolute_matrix;
		}
	}
	return model;
	*/
}
MeshModelNode* AssImp::loadModelNode( aiNode* ainode, MeshModelNode* parent, MeshModel* model ){
	assert( ainode ); assert( model );
	MeshModelNode* node = new MeshModelNode();
	model->_allnodes.add( node );
	node->_name = String( ainode->mName.C_Str() );
//	if( node->_name == model->id() )
//		node->_name = String();
	node->_relative_matrix = Mat4( aiMatrix4x4ToMat4( ainode->mTransformation ) );
	for( uint m = 0; m < ainode->mNumMeshes; m++ ){
		uint mi = ainode->mMeshes[ m ];
		assert( mi < model->_meshes.size() );
		auto mesh = model->_meshes.get( mi );
		//Material* material = null;
		//node->_meshes.add( new MeshModelMatMesh( material, mesh ) );
		node->_meshes.add( mesh );
//logDebug( "lmn ", ainode->mName.C_Str(), m , mi, _aiScene->mMeshes[ mi ]->mMaterialIndex,
//		  _aiScene->mMaterials[ _aiScene->mMeshes[ mi ]->mMaterialIndex ]->GetName().C_Str() );

//		if( node->_name.isEmpty() ){
//			uint meshindex = mi;
//			aiMesh* aimesh = _aiScene->mMeshes[ meshindex ];
//			aiMaterial* aimaterial = _aiScene->mMaterials[ aimesh->mMaterialIndex ];
//			node->_name = String( aimaterial->GetName().C_Str() );
//		}
	}
	if( parent == null ){
		assert( model->_root_node == null );
		model->_root_node = node;
	} else {
		node->_parent = parent;
		parent->_childs.add( node );
	}
	for( unsigned int c = 0; c < ainode->mNumChildren; c++ ){
		aiNode* aichild = ainode->mChildren[c];
		loadModelNode( aichild, node, model );
	}
	return node;
}

uint AssImp::meshCount() const
{
	assert( _aiScene );
    return _aiScene->mNumMeshes;
}
aiMesh *AssImp::aimesh( uint meshindex ){
    assert( _aiScene );
    assert( meshindex < _aiScene->mNumMeshes );
    aiMesh* aimesh = _aiScene->mMeshes[ meshindex ];
    return aimesh;
}
Mesh<VertexPNT>* AssImp::loadMeshPNT( uint meshindex ){
    aiMesh* aimesh = this->aimesh( meshindex );
    int invalid_normal_count = 0;
	/*
	if( aimesh->mPrimitiveTypes & aiPrimitiveType_POINT )
		logDebug( aimesh->mPrimitiveTypes, "aiPrimitiveType_POINT" );
	if( aimesh->mPrimitiveTypes & aiPrimitiveType_LINE )
		logDebug( aimesh->mPrimitiveTypes, "aiPrimitiveType_LINE" );
	if( aimesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE )
		logDebug( aimesh->mPrimitiveTypes, "aiPrimitiveType_TRIANGLE" );
	if( aimesh->mPrimitiveTypes & aiPrimitiveType_POLYGON )
		logDebug( aimesh->mPrimitiveTypes, "aiPrimitiveType_POLYGON" );
	if( aimesh->mPrimitiveTypes & aiPrimitiveType_NGONEncodingFlag )
		logDebug( aimesh->mPrimitiveTypes, "aiPrimitiveType_NGONEncodingFlag" );
		*/
	assert( aimesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE );
	int vertexCount = aimesh->mNumVertices;
	int indexCount = calcIndexCountForMesh( aimesh );
	Mesh<VertexPNT>* mesh = new Mesh<VertexPNT>( vertexCount, indexCount );
	aiMaterial* aimaterial = _aiScene->mMaterials[ aimesh->mMaterialIndex ];
	//mesh->setId( String( aimaterial->GetName().C_Str() ) );
	unsigned int pTextureCoordsIndex = 0;
	assert( aimesh->HasTextureCoords( pTextureCoordsIndex ) );
	assert( aimesh->mTextureCoords );
	aiVector3D* aitexturecoords = aimesh->mTextureCoords[ pTextureCoordsIndex ];
	VertexPNT vertex;
	for( int v = 0; v < vertexCount; v++ ){
		assert( aimesh->mVertices );
		assert( aimesh->mNormals );
		aiVector3D& aiPos = aimesh->mVertices[v];
		aiVector3D& aiNorm = aimesh->mNormals[v];
		const aiVector3D& aitexturecord = aitexturecoords[ v ];
		//const aiVector3D& aitexturecords = aimesh->mTextureCoords[v];
		assert( aitexturecord.z == 0.0f );
		Vec3 normal( aiNorm.x, aiNorm.y, aiNorm.z );
		if( normal.length2() < 0.001f ){
            logDebug( "AssImp::normal of vertex is null", v, normal.toString() );
			invalid_normal_count ++;
			normal = Vec3::Up;
		}
		vertex.position = _transform.map( Vec3( aiPos.x, aiPos.y, aiPos.z ) );
		vertex.normal = _transform.mapNormal( normal );
		vertex.texCoord.x = aitexturecord.x;
		vertex.texCoord.y = 1.0f - aitexturecord.y;
//		if( aimesh->mColors ){
//			const aiColor4D* aic = aimesh->mColors[0];
//			if( aic ){
//				const aiColor4D& aicr = aic[v];
//				std::cout << aicr.b;
//				if( aicr.b < 0.5f ){
//					vertex.texCoord.x = 104.0f / 4096;
//					vertex.texCoord.y = 140.0f / 4096;
//				} else {
//					vertex.texCoord.x = 1502.0f / 4096;
//					vertex.texCoord.y = 2168.0f / 4096;
//				}
//				//vertexx.texCoord.y = 1.0f - vertexx.texCoord.y;
//			}
//		}
		mesh->addVertex( vertex );
		// aiColor4D* aicolors = aimesh->mColors[0];
		// vertex._color = aiToQVector4D( aicolors[v] );
		// vertex._color = QVector4D( 1, 0, 0, 0.5 );
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
//		Mat4 pastm;
//		pastm.setRotationAtAxis( 90, Vec3::Right );
//		mesh->transform( pastm );
//		pastm.setToIdentity();
//		//pastm.setRotationAtAxis( 180, Vec3::Up );
//		mesh->transform( pastm );	
    if( 0 < invalid_normal_count ){
		logError( "AssImp::loadMeshPNT invalid normal count: ", invalid_normal_count );
	}
	return mesh;
}
vec4 AssImp::getVertexColor( uint meshindex, uint vertexindex ){
	assert( meshindex < _aiScene->mNumMeshes );
	aiMesh* aimesh = _aiScene->mMeshes[ meshindex ];
	assert( aimesh );
	assert( vertexindex < aimesh->mNumVertices );
	assert( aimesh->mColors );
	const aiColor4D* aicolorchannel = aimesh->mColors[0];
	assert( aicolorchannel );
	const aiColor4D& aicolor = aicolorchannel[ vertexindex ];
	return vec4( aicolor.r, aicolor.g, aicolor.b, aicolor.a );
}
Skeleton* AssImp::loadSkeleton(){
	Skeleton* skeleton = new Skeleton();
	loadSkeletonNode( skeleton, _aiScene->mRootNode, null );
	skeleton->transform( _transform );
	return skeleton;
}
void AssImp::loadSkeletonNode( Skeleton* skeleton, aiNode* node, SkeletonJoint* parent ){
	SkeletonJoint* joint = skeleton->createJoint( String( node->mName.C_Str() ), parent, Mat4( ai_mat4_cast( node->mTransformation ) ) );
	for( int c = 0; c < node->mNumChildren; c++ ){
		aiNode* child = node->mChildren[ c ];
		loadSkeletonNode( skeleton, child, joint );
	}
}
uint AssImp::animationCount() const {
	return _aiScene->mNumAnimations;
}
AssImpAnimation* AssImp::loadAnimation(){
	assert( animationCount() == 1 );
	uint animationIndex = 0;
	aiAnimation* aiAnimation = aiscene()->mAnimations[ animationIndex ];
	//stream << "AssImp::loadAnimation " << animationIndex << " " << aiAnimation->mName.C_Str() << "\n";
	assert( aiAnimation->mNumMeshChannels == 0 );
	assert( aiAnimation->mNumMorphMeshChannels == 0 );
	//  duration=" << aianimation->mDuration
	//  ticks per second=" << aianimation->mTicksPerSecond
	//float durationInTicks = aiAnimation->mDuration;
	ASSERT( 0 < aiAnimation->mTicksPerSecond ); // wenn 0 könnte es 25 sein https://www.youtube.com/watch?v=gnnoPaStVzg 5:25)
	//float duration = durationInTicks / aiAnimation->mTicksPerSecond;
	float tick_to_sec_factor = 1.0f  / aiAnimation->mTicksPerSecond;
	//AssImpSkeleton* sourceSkeleton = loadSkeleton();
	AssImpAnimation* animation = new AssImpAnimation();
	//logDebug( "animation", aiAnimation->mName.C_Str() );
	for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
		aiNodeAnim* aichannel = aiAnimation->mChannels[c];
		ASSERT( aichannel->mPreState == 0 && aichannel->mPostState == 0 );
		//logDebug( "  c ", c, aichannel->mNodeName.C_Str(), aichannel->mNumPositionKeys, aichannel->mNumRotationKeys, aichannel->mNumScalingKeys );
		for( unsigned int p = 0; p < aichannel->mNumScalingKeys; p++ ){
			aiVectorKey& scalkey = aichannel->mScalingKeys[p];
			ASSERT( 0 <= scalkey.mTime );
			ASSERT( glm::abs( scalkey.mValue.x - 1.0f ) < 0.001f );
			ASSERT( glm::abs( scalkey.mValue.y - 1.0f ) < 0.001f );
			ASSERT( glm::abs( scalkey.mValue.z - 1.0f ) < 0.001f );
		}
		for( unsigned int p = 0; p < aichannel->mNumPositionKeys; p++ ){
			aiVectorKey& poskey = aichannel->mPositionKeys[p];
			if( poskey.mValue.x < 0.00001f && poskey.mValue.y < 0.00001f && poskey.mValue.z < 0.00001f ){
			} else {
				//logDebug( "    p ", p, poskey.mValue.x, poskey.mValue.y, poskey.mValue.z );
				//assert( false );
			}
		}
		for( unsigned int r = 0; r < aichannel->mNumRotationKeys; r++ ){
			aiQuatKey& rotkey = aichannel->mRotationKeys[r];
			float time = rotkey.mTime * tick_to_sec_factor;
			AssImpAnimationFrame* frame = animation->findFrameStartingAt( time );
			if( frame == null ){
				frame = new AssImpAnimationFrame();
				frame->_starttime = time;
				animation->_frames.add( frame );
			}
		}
	}
	animation->_frames.sort( []( AssImpAnimationFrame* a, AssImpAnimationFrame* b ){
		if( a->_starttime < b->_starttime )return -1;return 1;
	});
	for( int a = 0; a < animation->_frames.size(); a++ ){
		AssImpAnimationFrame* frame = animation->_frames.get( a );
		if( a + 1 < animation->_frames.size() ){
			frame->_next = animation->_frames.get( a + 1 );
			frame->_duration = frame->_next->_starttime - frame->_starttime;
			assert( 0 < frame->_duration );
			frame->_endtime = frame->_starttime + frame->_duration;
		} else {
			frame->_next = null;
			frame->_duration = 0;
			frame->_endtime = frame->_starttime + frame->_duration;
		}
	}
	animation->_duration = animation->_frames.last()->_starttime;
	assert( animation->_frames.first()->_starttime == 0 ); assert( animation->_duration > 0 );
	/*
	for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
		SkinAnimationChannel* skinChannel = skinAnimation->_channels.get( c );
		SkinJoint* joint = sourceSkeleton->getJointByName( skinChannel->name() );
		if( root_channel == null ){
			root_channel = skinChannel;
			root_source_joint = joint;
		} else {
			if( joint->isChildOf( root_source_joint ) ){
			} else {
				root_channel = skinChannel;
				root_source_joint = joint;
			}
		}
	}
	*/
	//logDebug( "  a", animation->_frames.size() );
	for( AssImpAnimationFrame* frame : animation->_frames ){
		//logDebug( "    ", frame->_starttime );
		double ticktime = frame->_starttime / tick_to_sec_factor;
		frame->_skeleton = new Skeleton();
		loadSkeletonNode( frame->_skeleton, _aiScene->mRootNode, null );
		for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
			aiNodeAnim* aichannel = aiAnimation->mChannels[c];
			SkeletonJoint* joint = frame->_skeleton->getJointByName( String( aichannel->mNodeName.C_Str() ) );
			glm::quat rotation = aiAnimGetRotationAt( aichannel, ticktime );
			Vec3 translation = aiAnimGetPositionAt( aichannel, ticktime );
			joint->setRelativeMatrix( Mat4::Translation( translation ) * Mat4::Rotation( rotation ) );
		}
		frame->_skeleton->transform( _transform );
	}
	return animation;
}
glm::quat AssImp::aiAnimGetRotationAt( aiNodeAnim* aichannel, double time ){
	ASSERT( aichannel );
	ASSERT( 0 < aichannel->mNumRotationKeys );
	if( time < aichannel->mRotationKeys[0].mTime ){
		return glm::quat();
	}
	for( unsigned int r = 1; r < aichannel->mNumRotationKeys; r++ ){
		aiQuatKey& rotkey0 = aichannel->mRotationKeys[r-1];
		aiQuatKey& rotkey1 = aichannel->mRotationKeys[r];
		if( rotkey0.mTime <= time && time <= rotkey1.mTime ){
			double duration = rotkey1.mTime - rotkey0.mTime;
			ASSERT( 0 < duration );
			double localtime = time - rotkey0.mTime;
			ASSERT( 0 <= localtime && localtime <= duration );
			double f1 = localtime / duration;
			aiQuaternion v;
			aiQuaternion::Interpolate( v, rotkey0.mValue, rotkey1.mValue, f1 );
			return glm::quat( v.w, v.x, v.y, v.z );
		}
	}
	aiQuatKey& rotkeylast = aichannel->mRotationKeys[ aichannel->mNumRotationKeys -1 ];
	ASSERT( rotkeylast.mTime <= time );
	return glm::quat( rotkeylast.mValue.w, rotkeylast.mValue.x, rotkeylast.mValue.y, rotkeylast.mValue.z );
}
Vec3 AssImp::aiAnimGetPositionAt( aiNodeAnim* aichannel, double time ){
	ASSERT( aichannel );
	ASSERT( 0 < aichannel->mNumPositionKeys );
	for( unsigned int p = 1; p < aichannel->mNumPositionKeys; p++ ){
		aiVectorKey& poskey0 = aichannel->mPositionKeys[p-1];
		aiVectorKey& poskey1 = aichannel->mPositionKeys[p];
		if( poskey0.mTime <= time && time <= poskey1.mTime ){
			double duration = poskey1.mTime - poskey0.mTime;
			ASSERT( 0 < duration );
			double localtime = time - poskey0.mTime;
			ASSERT( 0 <= localtime && localtime <= duration );
			double f1 = localtime / duration;
			double f0 = 1.0 - f1;
			aiVector3D v0 = poskey0.mValue;
			v0 *= f0;
			aiVector3D v1 = poskey1.mValue;
			v1 *= f1;
			aiVector3D v;
			v += v0;
			v += v1;
			return Vec3( v.x, v.y, v.z );
		}
	}
	aiVectorKey& poskeylast = aichannel->mPositionKeys[ aichannel->mNumPositionKeys -1 ];
	ASSERT( poskeylast.mTime <= time );
	return Vec3( poskeylast.mValue.x, poskeylast.mValue.y, poskeylast.mValue.z );
}
/*

void AssImp::loadNodes( std::function<void ( uint meshIndex, const glm::mat4& localMatrix, const glm::mat4& globalMatrix )> loader ){
	loadNodes( loader, glm::mat4( 1 ), _aiScene->mRootNode );
}
void AssImp::loadNodes(std::function<void ( uint meshIndex, const glm::mat4& localMatrix, const glm::mat4& globalMatrix )> loader, const glm::mat4& parentMatrix, aiNode *node ){
	glm::mat4 localMatrix = aiMatrix4x4ToMat4( node->mTransformation );
	glm::mat4 globalMatrix = localMatrix * parentMatrix;
	for( uint m = 0; m < node->mNumMeshes; m++ ){
		loader( node->mMeshes[m], localMatrix, globalMatrix );
	}
	for( unsigned int c = 0; c < node->mNumChildren; c++ ){
		aiNode* child = node->mChildren[c];
		loadNodes( loader, globalMatrix, child );
	}
}
SceneDescription* AssImp::loadSceneDescription( const List<Mesh<VertexPNT> *>& meshes ){
	SceneDescriptionNode* root_node = loadSceneDesciptionNode( _aiScene->mRootNode, null, meshes );
	assert( root_node );
	SceneDescription* sceneDescription = new SceneDescription( root_node );
	for( auto mesh : meshes ){
		sceneDescription->_meshes.add( mesh );
	}
	return sceneDescription;
}
SceneDescriptionNode* AssImp::loadSceneDesciptionNode( aiNode* ainode, SceneDescriptionNode* parent, const List<Mesh<VertexPNT>*>& meshes ){
	std::string name( ainode->mName.C_Str() );
	glm::mat4 matrix_in_parent = aiMatrix4x4ToMat4( ainode->mTransformation );
	if( parent == null ){
		Mat4 s, r1, r2, t;
		s.setScale( 0.01f );
		r1.setRotationAtAxis( -90, Vec3::Up );
		r2.setRotationAtAxis( -90, Vec3::Forward );
		t = r2 * r1 * s;
		matrix_in_parent = t.asGlm() * matrix_in_parent;
		//matrix_in_parent = _transform.asGlm() * matrix_in_parent;
//		auto r = glm::angleAxis( glm::radians( 90.0f ), vec3( 1, 0, 0 ) )
//			* glm::angleAxis( glm::radians( 180.0f ), vec3( 0, 1, 0 ) );
//		const glm::mat4 rotmatrix = glm::toMat4( r );// * glm::toMat4( glm::angleAxis( glm::radians( 180.0f ), vec3( 0, 0, 1 ) ) );
//		const glm::mat4 matrix = rotmatrix * matrix_in_parent;
//////		vec3 translate( matrix[3] );
//////		const glm::quat rot( glm::toQuat( matrix ) );
//////	//	//		vec3 sx( matrix[0][0], matrix[1][0], matrix[2][0] );
//////	//	//		vec3 sy( matrix[0][1], matrix[1][1], matrix[2][1] );
//////	//	//		vec3 sz( matrix[0][2], matrix[1][2], matrix[2][2] );
//////	//	//		vec3 scaling( glm::length( sx ), glm::length( sy ), glm::length( sz ) );
//////		PosOri posori_in_parent( translate , rot  );
//////	//	//		_instances.add( posori, meshIndex );
		//matrix_in_parent = matrix;
	}
//	//matrix_in_parent = glm::scale( matrix_in_parent, vec3( s, s, s ) );
//	//PosOri posori_in_parent( matrix );
	//PosOri posori_in_parent( matrix_in_parent );
//	posori_in_parent.setPosition( posori_in_parent.position() * 0.0001f );
//	posori_in_parent.setPosition( vec3( 0, 0, 0 ) );
	SceneDescriptionNode* node = new SceneDescriptionNode( name, parent, matrix_in_parent );
	for( uint m = 0; m < ainode->mNumMeshes; m++ ){
		uint meshIndex = ainode->mMeshes[m];
		Mesh<VertexPNT>* mesh = meshes.get( meshIndex );
		assert( mesh );
		node->addMesh( mesh );
	}
	for( unsigned int c = 0; c < ainode->mNumChildren; c++ ){
		aiNode* aichild = ainode->mChildren[c];
		SceneDescriptionNode* child = loadSceneDesciptionNode( aichild, node, meshes );
		assert( child );
		assert( node->childs().contains( child ) );
	}
	return node;
}
Skin* AssImp::loadSkin(){
	SkinSkeleton* skeleton = loadSkinSkeleton();
	Skin* skin = new Skin( skeleton );
	assert( _aiScene );
	for( uint m = 0; m < _aiScene->mNumMeshes; m++ ){
		auto mesh = loadSkinMesh( m, skeleton );
		skin->_meshes.add( mesh );
	}
	skin->setInvMatrices();
	//stream << "AssImp::loadSkin bone-count=" << skin->skeleton()->root()->maxSkinMatrixIndex() << "\n";
	Mat4 matrix = _transform * skeleton->root()->relativeMatrix();
	skeleton->root()->setRelativeMatrix( matrix );
	skeleton->root()->recalcToRelativeMatrix();
	return skin;
}
SkinSkeleton *AssImp::loadSkinSkeleton(){
	SkinJoint* rootJoint = loadSkinJoint( _aiScene->mRootNode );
	//Mat4 matrix = _transform * rootJoint->relativeMatrix();
	//rootJoint->setRelativeMatrix( matrix );
	rootJoint->recalcToRelativeMatrix();
	SkinSkeleton* skeleton = new SkinSkeleton( rootJoint );
	return skeleton;
}
SkinJoint *AssImp::loadSkinJoint( aiNode* node ){
	String name = fixName( node->mName );
	SkinJoint* joint = new SkinJoint( name );
	joint->setRelativeMatrix( Mat4( aiMatrix4x4ToMat4( node->mTransformation ) ) );
	for( unsigned int c = 0; c < node->mNumChildren; c++ ){
		SkinJoint* child = loadSkinJoint( node->mChildren[c] );
		joint->addChild( child );
	}
	return joint;
}
Mesh<SkinVertex> *AssImp::loadSkinMesh( uint mesh_index, SkinSkeleton* skeleton ){
	assert( mesh_index < _aiScene->mNumMeshes );
	assert( skeleton );
	aiMesh* aimesh = _aiScene->mMeshes[ mesh_index ];
	assert( aimesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE );
	int vertexCount = aimesh->mNumVertices;
	int indexCount = aiIndexCount( aimesh );
	Mesh<SkinVertex>* mesh = new Mesh<SkinVertex>( vertexCount, indexCount );
	unsigned int pTextureCoordsIndex = 0;
	assert( aimesh->HasTextureCoords( pTextureCoordsIndex ) );
	assert( aimesh->mTextureCoords );
	aiVector3D* aitexturecoords = aimesh->mTextureCoords[ pTextureCoordsIndex ];
	for( int v = 0; v < vertexCount; v++ ){
//		if( v == 682 ){
//			stream << "";
//		}
		assert( aimesh->mVertices );
		assert( aimesh->mNormals );
		SkinVertex vertex;
		const aiVector3D& aiPos = aimesh->mVertices[v];
		const aiVector3D& aiNorm = aimesh->mNormals[v];
		const aiVector3D& aitexturecord = aitexturecoords[ v ]; assert( aitexturecord.z == 0.0f );
		vertex.position = _transform.map( Vec3( aiPos.x, aiPos.y, aiPos.z ) );
		Vec3 normal( aiNorm.x, aiNorm.y, aiNorm.z );
		if( normal.length2() < 0.001f ){
			static int inv_normal = 0;
			if( inv_normal < 1 ){
				stream << "AssImp::normal is null\n";
			}
			inv_normal++;
			vertex.normal = Vec3::Up;
		} else {
			vertex.normal = _transform.mapNormal( normal );
		}
		assert( Vec3( vertex.normal ).isValid() );
		vertex.texcoord = glm::vec2( aitexturecord.x, 1.0f-aitexturecord.y );
		for( int b = 0; b < 3; b++ ){
			vertex.bone_weights[b] = 0;
			vertex.bone_indexes[b] = 0;
		}

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
	/ *
	struct WeightsPerVertexAndBone {
		int boneIndex;
		float boneWeight;
	};
	struct WeightsPerVertex {
		int vertexIndex;
		QList<WeightsPerVertexAndBone*> weights;
	};
	QHash<int,WeightsPerVertex*> weightsPerVertexes;
	* /
	for( unsigned int b = 0; b < aimesh->mNumBones; b++ ){
		aiBone* aibone = aimesh->mBones[b];
		if( aibone->mNumWeights < 1 )
			continue; // bone hat keinen einfluss auf skin
		String name = fixName( aibone->mName );
		SkinJoint* joint = skeleton->getJointByName( name );
		int skinMatrixId = skeleton->getOrCreateSkinMatrixId( joint );
		for( unsigned int weightIndex = 0; weightIndex < aibone->mNumWeights; weightIndex++ ){
			aiVertexWeight* weights = &aibone->mWeights[ weightIndex ];
			uint vertexIndex = weights->mVertexId;
			float weight = weights->mWeight;
			if( 1.0f < weight ){
				if( 1.000001f < weight ){
					stream << "AssImp::weight etwas little ueber 1 " << weight << "\n";
				}
				weight = 1;
			}
			assert( 0 < weight && weight <= 1.0f );
			SkinVertex& vertex = mesh->vertex( vertexIndex );
			vertex.addBoneWeight( skinMatrixId, weight );
		}
	}
	for( uint vertexIndex = 0; vertexIndex < mesh->vertexCount(); vertexIndex++ ){
		SkinVertex& vertex = mesh->vertex( vertexIndex );
		vertex.fixBoneWeights();
	}
	/ *
	assert( weightsPerVertexes.size() == skinMesh->meshbuffer()->vertexBuffer().size() );
	for( int vertexIndex = 0; vertexIndex < skinMesh->meshbuffer()->vertexBuffer().size(); vertexIndex++ ){
		RnSkinVertex& vertex = meshbuffer->vertexBuffer().get( vertexIndex );
		WeightsPerVertex* weightsPerVertex = weightsPerVertexes.value( vertexIndex );
		assert( weightsPerVertex );
		for( int w = 1; w < weightsPerVertex->weights.size(); w++ ){
			WeightsPerVertexAndBone* wpvb0 = weightsPerVertex->weights.at( w -1 );
			WeightsPerVertexAndBone* wpvb1 = weightsPerVertex->weights.at( w );
			assert( wpvb0->boneWeight >= wpvb1->boneWeight );
			assert( wpvb0->boneIndex != wpvb1->boneIndex );
		}
		if( weightsPerVertex->weights.size() == 0 ){
			assert( false );
		} else if( weightsPerVertex->weights.size() == 1 ){
			vertex._boneinfos[0] = weightsPerVertex->weights.at( 0 )->boneIndex;
			vertex._boneinfos[1] = 1.0f;
			vertex._boneinfos[2] = -1;
		} else {
			vertex._boneinfos[0] = weightsPerVertex->weights.at( 0 )->boneIndex;
			vertex._boneinfos[1] = weightsPerVertex->weights.at( 0 )->boneWeight / ( weightsPerVertex->weights.at( 0 )->boneWeight + weightsPerVertex->weights.at( 1 )->boneWeight );
			vertex._boneinfos[2] = weightsPerVertex->weights.at( 1 )->boneIndex;
			assert( 0.5f <= vertex._boneinfos[1] );
		}
	}
	aiIndexToBuffer( aimesh, meshbuffer->indexBuffer() );
	skinMesh->fixErrors();
	skinMesh->validate();
	skin->addMesh( skinMesh );
	* /
	return mesh;
}
SkinAnimation* AssImp::loadAnimation( uint animationIndex ){
	assert( animationIndex < animationCount() );
	aiAnimation* aiAnimation = _aiScene->mAnimations[ animationIndex ];
	//stream << "AssImp::loadAnimation " << animationIndex << " " << aiAnimation->mName.C_Str() << "\n";
	assert( aiAnimation->mNumMeshChannels == 0 );
	assert( aiAnimation->mNumMorphMeshChannels == 0 );
	//  duration=" << aianimation->mDuration
	//  ticks per second=" << aianimation->mTicksPerSecond
	//float durationInTicks = aiAnimation->mDuration;
	ASSERT( 0 < aiAnimation->mTicksPerSecond );
	//float duration = durationInTicks / aiAnimation->mTicksPerSecond;
	float tick_to_sec_factor = 1.0f  / aiAnimation->mTicksPerSecond;
	SkinSkeleton* sourceSkeleton = loadSkinSkeleton();
	//x
	//	_skeleton->root()->recalcByRelativeMatrix();
	//	//Mat4 t = Mat4::RotationAtAxis( -90, Vec3::Right );// * Mat4::Scale( 0.01f );
	//	//_skeleton->root()->setAbsoluteMatrix( _skeleton->root()->absoluteMatrix() * t );


	SkinAnimation* skinAnimation = new SkinAnimation( sourceSkeleton );
	for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
		aiNodeAnim* aichannel = aiAnimation->mChannels[c];
		String name = fixName( aichannel->mNodeName );
		//stream << "  channel: " << name << "\n";
		assert( aichannel->mPreState == 0 );
		assert( aichannel->mPostState == 0 );
		skinAnimation->_channels.add( new SkinAnimationChannel( c, name ) );
	}
	for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
		aiNodeAnim* aichannel = aiAnimation->mChannels[c];
		for( unsigned int p = 0; p < aichannel->mNumScalingKeys; p++ ){
			aiVectorKey& scalkey = aichannel->mScalingKeys[p];
			ASSERT( 0 <= scalkey.mTime );
			ASSERT( glm::abs( scalkey.mValue.x - 1.0f ) < 0.001f );
			ASSERT( glm::abs( scalkey.mValue.y - 1.0f ) < 0.001f );
			ASSERT( glm::abs( scalkey.mValue.z - 1.0f ) < 0.001f );
		}
		for( unsigned int p = 0; p < aichannel->mNumPositionKeys; p++ ){
			aiVectorKey& poskey = aichannel->mPositionKeys[p];
			skinAnimation->createFrameIfNeeded( poskey.mTime * tick_to_sec_factor );
		}
		for( unsigned int r = 0; r < aichannel->mNumRotationKeys; r++ ){
			aiQuatKey& rotkey = aichannel->mRotationKeys[r];
			skinAnimation->createFrameIfNeeded( rotkey.mTime * tick_to_sec_factor );
		}
	}
	SkinAnimationChannel* root_channel = null;
	SkinJoint* root_source_joint = null;
	for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
		//aiNodeAnim* aichannel = aiAnimation->mChannels[c];
		SkinAnimationChannel* skinChannel = skinAnimation->_channels.get( c );
		//String nodeName( aichannel->mNodeName.C_Str() );
		SkinJoint* joint = sourceSkeleton->getJointByName( skinChannel->name() );
		if( root_channel == null ){
			root_channel = skinChannel;
			root_source_joint = joint;
		} else {
			if( joint->isChildOf( root_source_joint ) ){
			} else {
				root_channel = skinChannel;
				root_source_joint = joint;
			}
		}
	}
	//stream << "root " << root_channel->name() << " " << root_source_joint->name() << "\n"; stream.flush();
	for( SkinAnimationFrame* frame : skinAnimation->frames() ){
		double ticktime = frame->time() / tick_to_sec_factor;
		frame->_skeleton = sourceSkeleton->clone();
		frame->_pose.reserve( aiAnimation->mNumChannels );
		frame->_skeleton->root()->recalcToRelativeMatrix();
		for( uint c = 0; c < aiAnimation->mNumChannels; c++ ){
			aiNodeAnim* aichannel = aiAnimation->mChannels[c];
			SkinAnimationChannel* skinChannel = skinAnimation->_channels.get( c );
			//String nodeName( aichannel->mNodeName.C_Str() );
			SkinJoint* joint = frame->_skeleton->getJointByName( skinChannel->name() );
			Vec3 translation;
			//glm::quat rotation;
			{
				aiVector3D aiposition = aiAnimGetPositionAt( aichannel, ticktime );
				Vec3 position( aiTovec3( aiposition ) );
				aiQuaternion airotation = aiAnimGetRotationAt( aichannel, ticktime );
				glm::quat rotation( airotation.w, airotation.x, airotation.y, airotation.z );
				frame->_pose.setRotation( c, rotation );
				aiMatrix4x4 aimr( airotation.GetMatrix() );
				aiMatrix4x4 aimt;
				aiMatrix4x4::Translation( aiposition, aimt );
				aiMatrix4x4 am = aimt * aimr;
				Mat4 m( aiMatrix4x4ToMat4( am ) );
				//QMatrix4x4 massimp = aiMatrix4x4ToQMatrix4x4( am );
				//QMatrix4x4 m = massimp;
				translation = m.map( Vec3::Null );

				rotation = glm::quat_cast( m.asGlm() );
				//rotation = QQuaternion::fromRotationMatrix( m.normalMatrix() );
				// joint->setAbsoluteMatrix( m );
				Mat4 r = joint->relativeMatrix();
				glm::quat rq( airotation.w, airotation.x, airotation.y, airotation.z );
				Mat4 rm = Mat4( glm::mat4_cast( rq ) );
				//Mat4( aiMa)
				Vec3 p = r.map( Vec3::Null );
				if( root_channel == skinChannel ){
					// stream << " f " << frame->time() << " c " << c << " " << skinChannel->name() << " pos " << position.toCode() << "\n";
					//frame->_pose._rotationCount
					p = position;
				}

				//r = r * rm;
				r = Mat4::Translation( p ) * rm;
				joint->setRelativeMatrix( r );
			}
		}
		//frame->_skeleton->root()->recalcByRelativeMatrix();
	}
	return skinAnimation;
}

String AssImp::fixName( const aiString& src ) const {
	String name( src.C_Str() );
	String mixaamorigStr( "mixamorig:" );
	if( name.startsWidth( mixaamorigStr ) ){
		name = name.mid( mixaamorigStr.length() );
	}
	name = name.toLower();
	return name;
}
*/
