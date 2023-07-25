#include "stdodt.h"
#include "VulkanLightArray.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanTask.h"
#include "renderer/RenderLight.h"
//#include "VulkanRaytracer.h"
//#include "vulkan/OdtVulkan.h"
////#include "VulkanWindow.h"
//#include "import/AssImp.h"
//#include "math/OdtMath.h"
//#include "render/OdtRender.h"


VulkanLightArray::VulkanLightArray()
	:_commandPool( null )
	,_light_count( 0 )
{
	memset( &_lights, 0, sizeof( _lights ));
}
VulkanLightArray::~VulkanLightArray(){
	ASSERT( _commandPool == null );
}
void VulkanLightArray::create( VulkanCommandPool* commandPool ){
	assert( commandPool );
	ASSERT( _commandPool == null );
	_commandPool = commandPool;
}
void VulkanLightArray::destroy(){
	_buffer.clear();
	_commandPool = null;
}
void VulkanLightArray::set( uint index, const RenderLight& light ){
	assert( index < VulkanLight_MaxCount );
	if( light._last_vulkan_index == index ){
		if( light._last_vulkan_uc == light.updateCounter() ){
			return;
		}
	}
	_modified = true;
	VulkanLightData& data = _lights[ index ];
	data.position = light.position().toVec40();
	data.direction = light.direction().toVec40();
	data.color = light.color();
	data.range = light.range();
	data.angle = light.angle();
	switch( light.type() ){
	case RenderLight::Ambient:
		data.type = VulkanLightType_Ambient;
		data.range = 999999;
		break;
	case RenderLight::Directional:
		data.type = VulkanLightType_Directional;
		data.range = 999999;
		break;
	case RenderLight::Point:
		data.type = VulkanLightType_Point;
		break;
	case RenderLight::Spot:
		data.type = VulkanLightType_Spot;
		break;
	default:
		assert( false );
	}
	data.last = 5432;
	light._last_vulkan_index = index;
	light._last_vulkan_uc = light.updateCounter();
}
void VulkanLightArray::update( VulkanTask& task ){
	if( _modified ){
		if( !_buffer.isCreated() ){
			assert( VulkanLight_MaxCount * sizeof( VulkanLightData ) == sizeof( _lights ) );
			_buffer.create(
				sizeof( _lights ),
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				_commandPool->device()
			);
		}
		task.addLoadBuffer( _buffer, _lights, sizeof( _lights ) );
		_modified = false;
	}
}
