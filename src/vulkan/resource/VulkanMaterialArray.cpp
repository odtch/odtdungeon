#include "stdodt.h"
#include "VulkanMaterialArray.h"
#include "resource/Material.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanTask.h"
#include "vulkan/raytracer/VulkanRaytracerGlobals.h"
#include "vulkan/resource/VulkanMaterial.h"

VulkanMaterialArray::VulkanMaterialArray(){
	memset( &_materials, 0, sizeof( _materials ) );
}
VulkanMaterialArray::~VulkanMaterialArray(){
	if( _textures ){
		logError( "~VulkanMaterialArray not destroyed" );
	}
	destroy();
	ASSERT( _textures == null );
}
void VulkanMaterialArray::create( VulkanCommandPool* commandPool, VulkanTextureArray* textures ){
	assert( commandPool );
	assert( textures );
	assert( _textures == null );
	_textures = textures;
	_commandPool = commandPool;
}
void VulkanMaterialArray::destroy(){
	_materialCount = 0;
	_buffer.clear();
	_textures = null;
	_commandPool = null;
}
void VulkanMaterialArray::update( VulkanRaytracerGlobals& globals, VulkanTask& task ){
	_materials[1].last = 1212;
	//Resources* resources = Resources::Get();
	_materialCount = globals.materials().size(); //. resources->materialCount();
	assert( _materialCount <= MaxMaterialCount );
	if( _materialCount == 0 ){
		logError( "VulkanMaterialArray no materials" );
		return;
	}
	for( uint32_t index = 0; index < _materialCount; index++ ){
		//Material* material = resources->materials()[ index ];
		VulkanMaterial* material = globals.materials().get( index ); //resources->materials()[ index ];
		if( material == null ){
			logError( "VulkanMaterialArray destroyed material", index );
		} else if( !material->_modified ){
		} else {
			VulkanMaterialData& data = _materials[ index ];
			data.flags = material->flags();			
			data.tileCountX = material->tileCountX();
			data.tileCountY = material->tileCountY();
			if( material->hasTexture() ){
				data.texture1Index = _textures->getTextureIndex( material->texture() );
			} else {
				data.texture1Index = -1;
			}
			data.color = material->color();
	// 		data.transparency = material->transparency();
			//data.reflection = material->reflection();
	// 		data.emissiveTextureIndex = ( material->hasEmissiveTexture() ? _textures->textures().getTextureIndex( material->emissiveTexture() ) : -1 );
			material->_modified = false;
			_modified = true;

		}
	}
	if( !_modified )
		return;
	_modified = false;
	size_t buffersize = _materialCount * sizeof( Material );
	if( _buffer.isCreated() ){
		if( _buffer.size() != buffersize ){
			logError( "VulkanMaterialArray buffer size changed" );;
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
//	for( int m = 0; m < _material_count; m++ ){
//		std::cout << "Mat.col " << m << " " << glm::to_string( _materials[ m ].color ) << "\n";
//	}
	task.addLoadBuffer( _buffer, _materials, buffersize );
	//_buffer.load( *_commandPool, _materials, buffersize );
}
