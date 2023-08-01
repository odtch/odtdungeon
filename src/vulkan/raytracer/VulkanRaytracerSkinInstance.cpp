#include "stdodt.h"
#include "VulkanRaytracerSkinInstance.h"
#include "skin/Skin.h"
#include "skin/SkinMesh.h"
#include "VulkanRaytracerInstance.h"

VulkanRaytracerSkinInstance::VulkanRaytracerSkinInstance( VulkanRaytracerRenderLayer* layer, const Mat4& posori, Skin* skin )
	:RenderSkinInstance( posori, skin )
	,_layer( asserted( layer ) )
{
}
VulkanRaytracerSkinInstance::~VulkanRaytracerSkinInstance(){
	odelete( _mesh );
}
void VulkanRaytracerSkinInstance::createMesh( Mesh<VertexPNT>* mesh ){
	assert( mesh );
	assert( _mesh == null );
	_mesh = mesh;
	assert( _mesh->isEmpty() );
	const SkinMesh& srcmesh = skin()->type()->mesh();
	for( uint v = 0; v < srcmesh.vertexCount(); v++ ){
		const SkinVertex& src = srcmesh.vertex( v );
		_mesh->addVertex( VertexPNT( src.position, src.normal, src.texcoord ) );
	}
	for( uint i = 0; i < srcmesh.indexCount(); i++ ){
		_mesh->addIndex( srcmesh.index( i ) );
	}
}
void VulkanRaytracerSkinInstance::updateMeshInCPU(){
	assert( _mesh );
	SkinMesh& srcmesh = skin()->type()->mesh();
	for( uint v = 0; v < srcmesh.vertexCount(); v++ ){
		const SkinVertex& source_vertex = srcmesh.vertex( v );
		VertexPNT& target_vertex = _mesh->vertex( v );
		target_vertex.position = vec3( 0, 0, 0 );
		//target_vertex.normal = vec3( 0, 0, 0 );
		for( int bi = 0; bi < 3; bi++ ){
			int bone_index = source_vertex.bone_indexes[ bi ];
			if( 0 <= bone_index ){
				//assert( _skin_matrix_valid[ bone_index ] );
				//const Mat4& mat = _skin_matrices[ bone_index ];
				const Mat4& mat = skin()->bone_matrix( bone_index );
				target_vertex.position += ( ( mat.map      ( Vec3( source_vertex.position ) ) ) * source_vertex.bone_weights[ bi ] ).asGlm();
				//target_vertex.normal   += ( ( mat.mapNormal( Vec3( source_vertex.normal   ) ) ) * source_vertex.bone_weights[ bi ] ).asGlm();
			}
		}
		//target_vertex.normal = glm::normalize( target_vertex.normal );
	}
	_mesh->setModified();
	if( _pntinstance )
		_pntinstance->setPosOri( posori() );
}
