#include "stdodt.h"
#include "VulkanRenderableArray.h"
#include "resource/Material.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanTask.h"
////#include "VulkanWindow.h"
//#include "import/AssImp.h"
//#include "math/OdtMath.h"
//#include "render/OdtRender.h"


VulkanRenderableArray::VulkanRenderableArray()
{
	int todo_del_raytracer;
	memset( &_instances, 0, sizeof( _instances ) );
}
VulkanRenderableArray::~VulkanRenderableArray(){
	if( _commandPool ){
		logError( "~VulkanRenderableArray not destroyed" );
	}
	destroy();
	ASSERT( _commandPool == null );
}
void VulkanRenderableArray::create( VulkanCommandPool* commandPool ){
	assert( commandPool );
	assert( _commandPool == null );
	_commandPool = commandPool;
}
void VulkanRenderableArray::destroy(){
	_next_instance_index = 0;
	_buffer.clear();
	_commandPool = null;
}
void VulkanRenderableArray::reset(){
	_next_instance_index = 0;
}
uint32_t VulkanRenderableArray::add( const Mat4& posori, uint meshIndex, uint materialIndex, const vec4& color, float textureOffsetY, float textureTile ){
    // , const vec4& datas, int effect
	assert( _commandPool );
	assert( _next_instance_index < MaxInstanceCount );
	VulkanRenderableData& instance = _instances[ _next_instance_index ];
	instance.meshIndex = meshIndex;
	instance.materialIndex = materialIndex;
	//instance.flags = material->raytracingFlags();
	//instance.textureIndex = -1;
	//if( material->hasTexture() ){
// 		instance.textureIndex = _raytracer->textures().getTextureIndex( material->texture() );
// 	}
	instance.color = color;
	//instance.textureOffsetX = textureOffset.x;
	instance.textureOffsetY = textureOffsetY;
	instance.textureTile = textureTile;
// 	instance.datas = datas;
// 	instance.transparency = material->transparency();
// 	instance.reflection = material->reflection();
// 	instance.effect = effect;
// 	instance.emissiveTextureIndex = ( material->hasEmissiveTexture() ?
// 									_raytracer->_textures.getTextureIndex( material->emissiveTexture() ) : -1 );
// 	instance.viewport_index = material->viewportIndex();
	//std::cout << "i " << _next_instance_index << " " << instance.viewport_index << "\n";
	uint32_t index = _next_instance_index;
	_next_instance_index++;
	return index;
}
void VulkanRenderableArray::update( VulkanTask& task ){
	if( _next_instance_index == 0 ){
		std::cout << "VulkanRenderableArray::update no instances\n";
		std::cout.flush();
		return;
	}
	size_t buffersize = _next_instance_index * sizeof( VulkanRenderableData );
	if( _buffer.isCreated() ){
		if( _buffer.size() != buffersize ){
			_buffer.clear();
		}
	}
	if( !_buffer.isCreated() ){
		_buffer.create(
			buffersize,
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _commandPool->device()
		);
	}
//	for( uint i = 0; i < _next_instance_index; i++ ){
//		std::cout << "Inst.col " << i << " " << glm::to_string( _instances[i ].color ) << "\n";
//	}
//	std::cout << "VRI " << _instances[1].last << "\n";
//	_instances[1].meshIndex = 123;
//	_instances[1].materialIndex = 234;
//	_instances[1].dummy12 = 345;
//	_instances[1].last = 456;
	_instances[1].last = 2312; assert( _next_instance_index >= 1 );

//	_instances[0].color = vec4( 1, 2, 3, 4 );
//	_instances[1].color = vec4( 11, 12, 13, 14 );

	task.addLoadBuffer( _buffer, _instances, buffersize );
}
