#include "stdodt.h"
#include "VulkanRaytracerGlobals.h"
#include "resource/Material.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/resource/VulkanTextureArray.h"
#include "vulkan/command/VulkanTask.h"

VulkanRaytracerGlobals::VulkanRaytracerGlobals()
	:_commandPool( null )
	,_materials( VuklanRaytracerGlobalsData_MaxMaterialCount )
{
	memset( &_data, 0, sizeof( _data ));
	_materials.fill( null );
}
VulkanRaytracerGlobals::~VulkanRaytracerGlobals(){
	ASSERT( _commandPool == null );
}
void VulkanRaytracerGlobals::create( VulkanCommandPool* commandPool ){
	assert( commandPool );
	ASSERT( _commandPool == null );
	_commandPool = commandPool;
}
void VulkanRaytracerGlobals::destroy(){
	_buffer.clear();
	_commandPool = null;
}
void VulkanRaytracerGlobals::registerMaterial( Material* material ){
	ASSERT( material );
	ASSERT( material->materialindex() < _materials.size() );
	if( _materials.get( material->materialindex() ) == null ){
		_materials.set( material->materialindex(), material );
		if( _materialCount <= material->materialindex() )
			_materialCount = material->materialindex() +1;
	} else {
		ASSERT( _materials.get( material->materialindex() ) == material );
	}
}
void VulkanRaytracerGlobals::update( VulkanTask& task ){
	updateData();
	updateBuffer( task );
}
void VulkanRaytracerGlobals::updateData(){
	{
		_data.material_count = _materialCount;
		for( int m = 0; m < _materials.size(); m++ ){
			Material* material = _materials[ m ];
			if( material ){
				VulkanMaterialData& data = _data.materials[ m ];
				data.color = material->color().asGlm();
//				data.flags = material->flags();
//				data.texture1Index = material->texture().;
//				data.tileCountX;
//				data.tileCountY;
//				data.reflection;
			} else {
				//logError( "VulkanRaytracerGlobals too many materials" );
			}
		}
	}
}
void VulkanRaytracerGlobals::updateBuffer( VulkanTask& task ){
//void VulkanRaytracerGlobals::update(){
	if( !_buffer.isCreated() ){
		_buffer.create(
			sizeof( _data ),
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			_commandPool->device()
		);
	}
	_data.sample_batch++;
	if( _data.sample_batch < 0 || _data.sample_batch > 63 )
		_data.sample_batch = 0;
//	for( int v = 0; v < 3; v++ ){
////		_data.layers[v].reserve01 = 1000 + v * 100 + 1;
//		_data.layers[v].reserve92 = 1000 + v * 100 + 92;
//		_data.layers[v].reserve93 = 1000 + v * 100 + 93;
//		_data.layers[v].last = 1000 + v * 100 + 99;
//	}
//	for( int v = 0; v < _data.layer_count; v++ ){
//		//logDebug( "viewport", v, "p:", _data.viewports[v].camera_position, "d:", _data.viewports[v].camera_direction, "p:", _data.viewports[v].camera_projection, "t:", _data.viewports[v].tlas_index );
//	}
	_data.reserve92 = 1092;
	_data.reserve93 = 1093;
	_data.last = 1094;
	task.addLoadBuffer( _buffer, &_data, sizeof( _data ) );
	//_buffer.load( *_commandPool, &data, sizeof( data ) );
//}
}
void VulkanRaytracerGlobals::begin(){
	_data.layer_count = 0;
	_data.decal_count = 0;
	_data.light_count = 0;
}
//void VulkanRaytracerGlobals::addDecal( const Decal& decal, VulkanTextureArray& textures ){
//	if( VulkanRaytracerDecal_MaxCount <= data.decal_count ){
//		assert( false );
//		return;
//	}
//	VulkanRaytracerDecal& v = data.decals[ data.decal_count ];
//	data.decal_count++;
//	if( decal.hasMaterial() ){
//		v.material_index = decal.material()->index();
//	} else {
//		v.material_index = -1;
//	}
//	v.position = decal.posori().position();
//	v.radius = decal.radius();
//	v.radius2 = decal.radius2();
//	v.textureTile = decal.textureTile();
//	v.inv_transform = glm::inverse( decal.posori().matrix().asGlm() );
//	v.color = decal.color();
//	//v.inv_transform = decal.posori().matrix().asGlm();
//	//* glm::orthoLH( -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1000.0f ) );


//	Camera c;
//	c.setPosOri( decal.posori() );
//	//c.setProjectionOrthographic( Rect2i( -10, -10, 20, 20 ) );
//	//c.set
//	c.recalcMatrices( Vec2( 100, 100 ) );
//	c.recalcTest();
//	v.inv_transform = c.projection_matrix().asGlm();
//	v.inv_transform = c.view_matrix().asGlm();
//	//v.inv_transform = glm::inverse( v.inv_transform );

//	PosOri po = decal.posori();
//	v.inv_transform = glm::lookAtRH( po.position().asGlm(), po.position().asGlm() + po.direction().asGlm(), po.up().asGlm() );


//	v.last = 2071;
//}

