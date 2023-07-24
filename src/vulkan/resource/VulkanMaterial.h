#pragma once

#include "resource/Material.h"
#include "vulkan/VulkanCore.h"

class VulkanMaterial : public Material
{
private:
public:
	explicit VulkanMaterial( const String& name );
	virtual ~VulkanMaterial() override;
};
