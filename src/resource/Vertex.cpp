#include "stdodt.h"
#include "Vertex.h"
#include "Mesh.h"
#include "ResourceType.h"

//ResourceType* VertexP::GetResourceType(){
//	assert( false );
//}
ResourceType* VertexPNT::GetResourceType(){
	return Singleton::Get<MeshPNTType>();
}

SkinVertex::SkinVertex()
	:position( 0, 0, 0 )
	,normal( 0, 0, 0 )
	,texcoord( 0, 0 )
{
	for( int b = 0; b < 3; b++ ){
		bone_weights[b] = 0;
		bone_indexes[b] = -1;
	}
}
void SkinVertex::addBoneWeight( int boneindex, float weight ){
	assert( 0 <= boneindex );
	assert( 0 < weight && weight <= 1.0001f );
	for( int i = 0; i < 3; i++ ){
		if( bone_indexes[ i ] == -1 ){
			bone_indexes[ i ] = boneindex;
			bone_weights[ i ] = weight;
			return;
		}
	}
	for( int i = 0; i < 3; i++ ){
		if( bone_weights[ i ] < weight ){
			bone_indexes[ i ] = boneindex;
			bone_weights[ i ] = weight;
			return;
		}
	}
}
float SkinVertex::totalBoneWeight() const {
	float sum = 0;
	for( int i = 0; i < 3; i++ ){
		if( bone_indexes[ i ] < 0 )
			break;
		ASSERT( 0 < bone_weights[ i ] );
		sum += bone_weights[ i ];
	}
	return sum;
}
void SkinVertex::adjustBoneWeightsTo1(){
	float sum = totalBoneWeight();
	assert( 0.001f < sum && sum < 1.1f );
	float f = 1.0f / sum;
	for( int i = 0; i < 3; i++ ){
		if( 0 <= bone_indexes[ i ]  ){
			bone_weights[ i ] *= f;
		}
	}
	sum = totalBoneWeight();
	assert( 0.9999f < sum && sum < 1.0001f );
}
ResourceType* SkinVertex::GetResourceType(){
	assert( false );
}
