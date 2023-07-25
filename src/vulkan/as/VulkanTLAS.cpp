#include "stdodt.h"
#include "VulkanTLAS.h"
#include "VulkanBLAS.h"
#include "VulkanAccelerationStructureExtension.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanMeshBuffer.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

class VulkanTLASUpdateCommand : public VulkanCommand
{
private:
	VulkanTLAS* _tlas;
public:
	explicit VulkanTLASUpdateCommand( VulkanTLAS* tlas );
	virtual ~VulkanTLASUpdateCommand() override;
public:
	virtual void bind( VulkanCommandBuffer& commandBuffer ) override;
	virtual void onCompleted() override;
};
VulkanTLASUpdateCommand::VulkanTLASUpdateCommand( VulkanTLAS* tlas )
	:_tlas( tlas )
{
	assert( _tlas );
}
VulkanTLASUpdateCommand::~VulkanTLASUpdateCommand(){
	_tlas = null;
}
void VulkanTLASUpdateCommand::bind( VulkanCommandBuffer& commandBuffer ){
	ASSERT( !_tlas->_rebuilding );
	_tlas->_rebuilding = true;
}
void VulkanTLASUpdateCommand::onCompleted(){
//	_buffer.onLoadCompleted();
	ASSERT( _tlas->_rebuilding );
	while( !_tlas->_buildingBlases.isEmpty() ){
		VulkanBLAS* blas = _tlas->_buildingBlases.takeFirst();
		ASSERT( blas->_rebuildingTLAS );
		blas->_rebuildingTLAS = false;
	}
	_tlas->_rebuilding = false;
	_tlas = null;
}

VulkanTLAS::VulkanTLAS( uint32_t index )
	:_index( index )
{
	memset( &_instances, 0, sizeof( _instances ) );
}
VulkanTLAS::~VulkanTLAS(){
	_stagingBuffer.clear();
}
void VulkanTLAS::reset(){
	_instance_count = 0;
	ASSERT( _buildingBlases.isEmpty() );
}
void VulkanTLAS::add( const Mat4& transform, uint32_t instanceIndex, uint32_t mask, uint32_t instanceShaderBindingTableRecordOffset, VkGeometryInstanceFlagsKHR flags, VulkanBLAS* blas ){
	assert( blas );
	assert( _instance_count < MaxInstanceCount );
	//ASSERT( !blas->_rebuildingTLAS );
	if( !blas->_rebuildingTLAS ){
		blas->_rebuildingTLAS = true;
		_buildingBlases.add( blas );
	} else {

	}
	VkAccelerationStructureInstanceKHR& instance = _instances[ _instance_count ];
	_instance_count++;
	glm::mat4 transp = glm::transpose( transform.asGlm() );
	memcpy( &instance.transform, &transp, sizeof(instance.transform) );
	instance.instanceCustomIndex                    = instanceIndex;
	instance.mask                                   = mask;
	instance.instanceShaderBindingTableRecordOffset = instanceShaderBindingTableRecordOffset;

//    static int test = 0;
//    test++;
//    instance.instanceShaderBindingTableRecordOffset = test%2;


	instance.flags                                  = flags;
	instance.accelerationStructureReference         = blas->buffer().getBufferDeviceAddress();
}
void VulkanTLAS::addRebuild( VulkanTask& loadtask, VulkanTask& buildtask ){
	if( _instance_count == 0 ){
		logDebug( "VulkanTLAS::update empty" );
		return;
	}
	if( isCreated() ){
		if( _instance_count != _prev_count ){
			logDebug( "VulkanTLAS::update count changed", _instance_count, _prev_count );
			destroy();
		}
	}
	bool create = !isCreated();
	if( create ){
		bindDevice( loadtask.device() );
	}
	_prev_count = _instance_count;
	VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR
			| VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
	struct Tlas {
		  VkAccelerationStructureKHR as;
	//	  VkBuildAccelerationStructureFlagsKHR flags = 0;
	};
	// Create a buffer holding the actual instance data (matrices++) for use by the AS builder
	VkDeviceSize instanceDescsSizeInBytes = _instance_count * sizeof( VkAccelerationStructureInstanceKHR );
	// Allocate the instance buffer and copy its contents from host to device memory
	if( _instanceBuffer.isCreated() ){
		if( _instanceBuffer.size() < instanceDescsSizeInBytes ){
			std::cout << "VulkanTLAS.instanceBuffer.recreate to small\n";
			_instanceBuffer.clear();
		}
	}
	if( !_instanceBuffer.isCreated() ){
		_instanceBuffer.create( instanceDescsSizeInBytes, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		| VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, loadtask.device() );
	}
	if( _stagingBuffer.isCreated() ){
		if( _stagingBuffer.size() < instanceDescsSizeInBytes ){
			_stagingBuffer.clear();
		}
	}
	if( !_stagingBuffer.isCreated() ){
		_stagingBuffer.createStaging( &_instances, instanceDescsSizeInBytes, device() );
	} else {
		_stagingBuffer.loadStaging( &_instances, instanceDescsSizeInBytes );
	}
	{
//		task.addLoadBuffer( _instanceBuffer, &_instances, instanceDescsSizeInBytes );
		VkBufferCopy region{0, 0, instanceDescsSizeInBytes };
		vkCmdCopyBuffer( loadtask.commandBuffer().vkCommandBuffer(), _stagingBuffer.vkBuffer(), _instanceBuffer.vkBuffer(), 1, &region);
//		commandBuffer.end();
//		commandBuffer.execute();
//		commandBuffer.destroy();
//		_stagingBuffer.clear();
	}
////	VulkanCommandBuffer commandBuffer;
////	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );
//	// Make sure the copy of the instance buffer are copied before triggering the acceleration structure build
//	VkMemoryBarrier barrier;
//	memset( &barrier, 0, sizeof( barrier ) );
//	barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
//	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//	barrier.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
//	vkCmdPipelineBarrier( task.commandBuffer().vkCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
//						 0, 1, &barrier, 0, nullptr, 0, nullptr);
	//--------------------------------------------------------------------------------------------------
	// Create VkAccelerationStructureGeometryInstancesDataKHR
	// This wraps a device pointer to the above uploaded instances.
	VkAccelerationStructureGeometryInstancesDataKHR instancesVk;
	memset( &instancesVk, 0, sizeof( instancesVk ) );
	instancesVk.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	instancesVk.arrayOfPointers    = VK_FALSE;
	instancesVk.data.deviceAddress = _instanceBuffer.getBufferDeviceAddress();
	// Put the above into a VkAccelerationStructureGeometryKHR. We need to put the
	// instances struct in a union and label it as instance data.
	VkAccelerationStructureGeometryKHR topASGeometry;
	memset( &topASGeometry, 0, sizeof( topASGeometry ) );
	topASGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	topASGeometry.geometryType       = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	topASGeometry.geometry.instances = instancesVk;
	// Find sizes
	VkAccelerationStructureBuildGeometryInfoKHR buildInfo;
	memset( &buildInfo, 0, sizeof( buildInfo ) );
	buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	buildInfo.flags         = flags;
	buildInfo.geometryCount = 1;
	buildInfo.pGeometries   = &topASGeometry;
	buildInfo.mode = create ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
	buildInfo.type                     = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	buildInfo.srcAccelerationStructure = VK_NULL_HANDLE;
	uint32_t count = _instance_count;
	VkAccelerationStructureBuildSizesInfoKHR sizeInfo;
	memset( &sizeInfo, 0, sizeof( sizeInfo ) );
	sizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	extension()->getAccelerationStructureBuildSizesKHR()( buildtask.device()->vkDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &buildInfo, &count, &sizeInfo);
	// Create TLAS
	if( create ){
		VulkanAccelerationStructure::create( VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, sizeInfo.accelerationStructureSize );
	} else {
	}
	// Allocate the scratch memory
	if( _scratchBuffer.isCreated() ){
		if( _scratchBuffer.size() < sizeInfo.buildScratchSize ){
			_scratchBuffer.clear();
		}
	}
	if( !_scratchBuffer.isCreated() ){
		_scratchBuffer.create( sizeInfo.buildScratchSize,
							  VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
							  | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
							  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device() );
	}
	// Update build information
	VkAccelerationStructureKHR oldHandle = vkAccelerationStructure();
	buildInfo.srcAccelerationStructure  = create ? VK_NULL_HANDLE : vkAccelerationStructure();
	buildInfo.dstAccelerationStructure  = vkAccelerationStructure();
	buildInfo.scratchData.deviceAddress = _scratchBuffer.getBufferDeviceAddress();
	// Build Offsets info: n instances
	VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo{ _instance_count, 0, 0, 0 };
	const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;
	// Build the TLAS
	extension()->cmdBuildAccelerationStructuresKHR()( buildtask.commandBuffer().vkCommandBuffer(), 1, &buildInfo, &pBuildOffsetInfo);
//	commandBuffer.end();
//	commandBuffer.execute();
//	commandBuffer.destroy();
//	scratchBuffer.clear();
	VkAccelerationStructureKHR newHandle = vkAccelerationStructure();
	assert( oldHandle == newHandle );
	buildtask.add( new VulkanTLASUpdateCommand( this ) );
//	return true;
}
void VulkanTLAS::destroy(){
	VulkanAccelerationStructure::destroy();
	_instanceBuffer.clear();
	_stagingBuffer.clear();
}
