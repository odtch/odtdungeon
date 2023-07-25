#include "stdodt.h"
#include "VulkanBLAS.h"
#include "VulkanAccelerationStructureExtension.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/resource/VulkanMeshBuffer.h"
#include "vulkan/command/VulkanCommandPool.h"
#include "vulkan/command/VulkanCommandBuffer.h"

class VulkanBLASUpdateCommand : public VulkanCommand
{
private:
	VulkanBLAS* _blas;
	VulkanBuffer _scratchBuffer;
public:
	explicit VulkanBLASUpdateCommand( VulkanBLAS* blas );
	virtual ~VulkanBLASUpdateCommand() override;
public:
	virtual void bind( VulkanCommandBuffer& commandBuffer ) override;
	virtual void onCompleted() override;
};
VulkanBLASUpdateCommand::VulkanBLASUpdateCommand( VulkanBLAS* blas )
	:_blas( blas )
{
	assert( _blas );
}
VulkanBLASUpdateCommand::~VulkanBLASUpdateCommand(){
	_blas = null;
}
void VulkanBLASUpdateCommand::bind( VulkanCommandBuffer& commandBuffer ){
	ASSERT( !_blas->_rebuilding );
	_blas->_rebuilding = true;
	// Estimate the amount of scratch memory required to build the BLAS, and
	// update the size of the scratch buffer that will be allocated to
	// sequentially build all BLASes
//	VkAccelerationStructureMemoryRequirementsInfoKHR memoryRequirementsInfo;
//	memset( &memoryRequirementsInfo, 0, sizeof( memoryRequirementsInfo ));
//	memoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_KHR;
//	memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_BUILD_SCRATCH_KHR;
//	memoryRequirementsInfo.accelerationStructure = _vkAccelerationStructure;
//	memoryRequirementsInfo.buildType             = VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR;
//	VkMemoryRequirements2 reqMem;
//	memset( &reqMem, 0, sizeof( reqMem ) );
//	reqMem.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
//	PFN_vkGetAccelerationStructureMemoryRequirementsKHR GetAccelerationStructureMemoryRequirementsKHR = reinterpret_cast<PFN_vkGetAccelerationStructureMemoryRequirementsKHR>(
//															  vkGetDeviceProcAddr( device()->vkDevice(), "vkGetAccelerationStructureMemoryRequirementsKHR" ) );
//	assert( GetAccelerationStructureMemoryRequirementsKHR );
//	GetAccelerationStructureMemoryRequirementsKHR( device()->vkDevice(), &memoryRequirementsInfo, &reqMem );
//	VkDeviceSize scratchSize = reqMem.memoryRequirements.size;
//	VkDeviceSize maxScratch{0};  // Largest scratch buffer for our BLAS
//	//blas.flags = flags;
//	maxScratch = std::max(maxScratch, scratchSize);
//	// Original size
//	memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_KHR;
//	GetAccelerationStructureMemoryRequirementsKHR( device()->vkDevice(), &memoryRequirementsInfo, &reqMem );
//	std::vector<VkDeviceSize> originalSizes;
//	originalSizes.resize( 1 );
//	originalSizes[0] = reqMem.memoryRequirements.size;
//	//	  idx++;
//	//	  }
	// Allocate the scratch buffers holding the temporary data of the acceleration structure builder
	_scratchBuffer.create( _blas->_sizeInfo.buildScratchSize,
						  //VK_BUFFER_USAGE_RAY_TRACING_BIT_KHR |
						  VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
						  VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _blas->device() );
////	VkBufferDeviceAddressInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO};
////	bufferInfo.buffer              = scratchBuffer.buffer;
//	VkDeviceAddress scratchAddress = scratchBuffer.getBufferDeviceAddress();//vkGetBufferDeviceAddress(m_device, &bufferInfo);
//	assert( scratchAddress );
////	// Query size of compact BLAS
////	VkQueryPoolCreateInfo qpci{VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO};
////	qpci.queryCount = (uint32_t)m_blas.size();
////	qpci.queryType  = VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
////	VkQueryPool queryPool;
////	vkCreateQueryPool(m_device, &qpci, nullptr, &queryPool);


//	// Query size of compact BLAS
//	VkQueryPoolCreateInfo qpci;
//	memset( &qpci, 0, sizeof( qpci ) );
//	qpci.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
//	qpci.queryCount = (uint32_t)1;
//	qpci.queryType  = VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
//	VkQueryPool queryPool = nullptr;
//	Vulkan::check( vkCreateQueryPool( device()->vkDevice(), &qpci, nullptr, &queryPool), "vkCreateQueryPool failed" );
//	assert( queryPool );

//	VulkanCommandPool genCmdBuf;
//	genCmdBuf.create( device()->getQueue( device()->physicalDevice()->getGraphicsQueueFamily() ));
//	std::vector<VkCommandBuffer> allCmdBufs;
//	allCmdBufs.reserve( 1 );

//	VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
//	VulkanCommandBuffer commandBuffer;
//	commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, &commandPool );

	//const VkAccelerationStructureGeometryKHR* const ppGeometries = &geometry;
//	VkAccelerationStructureBuildGeometryInfoKHR bottomASInfo;
//	memset( &bottomASInfo, 0, sizeof( bottomASInfo ));
//	bottomASInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
//	bottomASInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
//	bottomASInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
//	bottomASInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
//	bottomASInfo.srcAccelerationStructure = VK_NULL_HANDLE;
	_blas->_buildGeometryInfo.dstAccelerationStructure = _blas->vkAccelerationStructure();
	//bottomASInfo.geometryCount = 1;
	//buildGeometryInfo.ppGeometries = &ppGeometries;
	_blas->_buildGeometryInfo.scratchData.deviceAddress = _scratchBuffer.getBufferDeviceAddress();

//	  // Pointers of offset
//	  std::vector<const VkAccelerationStructureBuildOffsetInfoKHR*> pBuildOffset(1);
//		pBuildOffset[0] = &offset;

//	  // Building the AS
//		PFN_vkCmdBuildAccelerationStructureKHR CmdBuildAccelerationStructureKHR = reinterpret_cast<PFN_vkCmdBuildAccelerationStructureKHR>(
//			   vkGetDeviceProcAddr( device()->vkDevice(), "vkCmdBuildAccelerationStructureKHR" ) );
//		assert( CmdBuildAccelerationStructureKHR );

	const VkAccelerationStructureBuildRangeInfoKHR* const ppBuildRangeInfos = &_blas->_offsetInfo;

	_blas->extension()->cmdBuildAccelerationStructuresKHR()( commandBuffer.vkCommandBuffer(), 1, &_blas->_buildGeometryInfo, &ppBuildRangeInfos );

////	  // Since the scratch buffer is reused across builds, we need a barrier to ensure one build is finished before starting the next one
//	  VkMemoryBarrier barrier;
//	  memset( &barrier, 0, sizeof( barrier ) );
//	  barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
//	  barrier.srcAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
//	  barrier.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
//	  vkCmdPipelineBarrier( commandBuffer.vkCommandBuffer(), VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
//						   VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR, 0, 1, &barrier, 0, nullptr, 0, nullptr);
////	  bool doCompaction = true;
////	  // Query the compact size
////	  if(doCompaction)
////	  {
//////		vkCmdWriteAccelerationStructuresPropertiesKHR(cmdBuf.vkCommandBuffer(), 1, &accel,
//////													  VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, queryPool,
//////													  1);
////	  }
}
void VulkanBLASUpdateCommand::onCompleted(){
//	_buffer.onLoadCompleted();
	ASSERT( _blas->_rebuilding );
	_blas->_rebuilding = false;
	_scratchBuffer.clear();
	_blas = null;
}

VulkanBLAS::VulkanBLAS(){
	memset( &_sizeInfo, 0, sizeof( _sizeInfo ) );
	memset( &_buildGeometryInfo, 0, sizeof( _buildGeometryInfo ));
	memset( &_offsetInfo, 0, sizeof( _offsetInfo ));
	memset( &_geometry, 0, sizeof( _geometry ) );
}
VulkanBLAS::~VulkanBLAS(){
}
void VulkanBLAS::setRebuildNeeded(){
	_rebuildNeeded = true;
}
void VulkanBLAS::addRebuild( VulkanTask& task, VulkanMeshBuffer& mesh ){
	if( isCreated() ){
		//throw std::runtime_error( "VulkanAccelerationStructure already created" );
		//std::cout << "VulkanBLAS::create update\n";
	} else {
		bindDevice( task.device() );
	}
	memset( &_geometry, 0, sizeof( _geometry ) );
	_geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	_geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
	_geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	_geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
	_geometry.geometry.triangles.vertexData.deviceAddress = mesh.vertexBuffer().getBufferDeviceAddress();
	_geometry.geometry.triangles.vertexStride = mesh.vertexSize();
	int TODO;
	_geometry.geometry.triangles.maxVertex = mesh.vertexCount() -1; // -1 ???
	_geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
	_geometry.geometry.triangles.indexData.deviceAddress = mesh.indexBuffer().getBufferDeviceAddress();
	//geometry.geometry.triangles.transformData.deviceAddress = 0;  // No transform
	_geometry.flags = mesh.geometryFlags();
	// Create offset info that allows us to say how many triangles and vertices to read
	memset( &_offsetInfo, 0, sizeof( _offsetInfo ));
	_offsetInfo.firstVertex = 0;
	_offsetInfo.primitiveCount  = mesh.indexCount() / 3;
	_offsetInfo.primitiveOffset = 0;
	_offsetInfo.transformOffset = 0;
	// blas
	memset( &_buildGeometryInfo, 0, sizeof( _buildGeometryInfo ));
	_buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	_buildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	_buildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	_buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	_buildGeometryInfo.geometryCount = 1;
	_buildGeometryInfo.pGeometries = &_geometry;
	_buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
	// Query both the size of the finished acceleration structure and the  amount of scratch memory
	// needed (both written to sizeInfo). The `vkGetAccelerationStructureBuildSizesKHR` function
	// computes the worst case memory requirements based on the user-reported max number of
	// primitives. Later, compaction can fix this potential inefficiency.
	uint32_t maxPrimCount = _offsetInfo.primitiveCount;
	memset( &_sizeInfo, 0, sizeof( _sizeInfo ) );
	_sizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	extension()->getAccelerationStructureBuildSizesKHR()(
		device()->vkDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
		&_buildGeometryInfo, &maxPrimCount, &_sizeInfo
	);
	// Create acceleration structure object. Not yet bound to memory.
	if( !isCreated() ){
		VulkanAccelerationStructure::create( VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR, _sizeInfo.accelerationStructureSize );
	} else {
	}
	task.add( new VulkanBLASUpdateCommand( this ) );
	_rebuildNeeded = false;
}
void VulkanBLAS::destroy(){
	VulkanAccelerationStructure::destroy();
}

