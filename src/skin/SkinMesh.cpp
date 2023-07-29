#include "stdodt.h"
#include "SkinMesh.h"
#include "resource/MeshBuilder.h"

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


//Mesh<VertexPNT>* SkinMesh::Convert( const Mesh<SkinVertex>& source ){
//    return MeshBuilder::Convert<SkinVertex,VertexPNT>( source, []( VertexPNT& t, const SkinVertex& s ){
//		t.position = s.position;
//		t.normal = s.normal;
//		t.texCoord = s.texcoord;
//	});
//}
//void SkinMesh::Update(const Mesh<SkinVertex>& source, Mat4* matrices, Mesh<VertexPNT>& target ){
//	const SkinVertex* source_vertex = &source.vertex( 0 );
//	VertexPNT* target_vertex = &target.vertex( 0 );
//	assert( source.vertexCount() == target.vertexCount() );
//	assert( source.indexCount() == target.indexCount() );
//	for( uint vertex_index = 0; vertex_index < source.vertexCount(); vertex_index++ ){
//		assert( source_vertex == &source.vertex( vertex_index ) );
//		target_vertex->position = Vec3::Null;
//		target_vertex->normal = Vec3::Null;
//		for( int bi = 0; bi < 3; bi++ ){
//			//if( 0 < source_vertex->bone_weights[ bi ] )
//			{
//				int bone_index = source_vertex->bone_indexes[ bi ];
//				//assert( skin_matrix_valid[bone_index]);
//				const Mat4& sm = matrices[ bone_index ];
////					if( 680 < vertex_index && vertex_index < 685 ){
////						std::cout << " t: " << mesh_index << " " << vertex_index << " " << Vec3( target_vertex->position   ).toCode() << " " << Vec3( target_vertex->normal   ).toCode() << "\n"; std::cout.flush();
////						std::cout << " a: " << mesh_index << " " << vertex_index << " " << Vec3( source_vertex->position   ).toCode() << " " << Vec3( source_vertex->normal   ).toCode() << "\n"; std::cout.flush();
////					}
//				target_vertex->position += sm.map      ( Vec3( source_vertex->position ) ).asGlm() * source_vertex->bone_weights[ bi ];
//				target_vertex->normal   += sm.mapNormal( Vec3( source_vertex->normal   ) ).asGlm() * source_vertex->bone_weights[ bi ];
//			}
//		}
//		source_vertex++;
//		target_vertex++;
//	}
//	target.setModified();
//}


//ResourceType* SkinVertex::GetResourceType()
//{
//	assert( false );
//}

SkinType::SkinType(){
}
SkinType::~SkinType(){
	odelete( _mesh );
	_bones.deleteAll();
	//odelete( _skeleton );
}
void SkinType::load( BinaryFileReader& reader ){
	assert( _mesh == null && _bones.isEmpty() );
	_mesh = new SkinMesh();
	_mesh->load( reader );
	uint32_t bonecount = reader.read_uint32();
	assert( 0 < bonecount && bonecount <= SkinMaxBoneCount );
	while( _bones.size() < bonecount ){
		SkinBoneType* bone = new SkinBoneType();
		bone->index = _bones.size();
		_bones.add( bone );
		reader.read_string( bone->name );
		reader.read_string( bone->jointname );
		bone->bone2mesh.load( reader );
	}
}
void SkinType::save( BinaryFileWriter& writer ) const {
	assert( _mesh );
	_mesh->save( writer );
	writer.write_uint32( _bones.size() );
	for( SkinBoneType* bone : _bones ){
		writer.write_string( bone->name );
		writer.write_string( bone->jointname );
		bone->bone2mesh.write( writer );
	}
}
