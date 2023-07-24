#pragma once

#include "vulkan/VulkanCore.h"

class VulkanCommand : public SharedPointerData
{
	DEBUGCOUNTER(VulkanCommand);
	ODTNOCOPY(VulkanCommand);
private:	
public:
	explicit VulkanCommand();
	virtual ~VulkanCommand();
public:
	virtual void bind( VulkanCommandBuffer& commandBuffer ) = 0;
	virtual void onCompleted() = 0;
};
