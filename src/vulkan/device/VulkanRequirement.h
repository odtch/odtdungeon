#pragma once

#include "vulkan/VulkanCore.h"

#define VULKAN_REQUIREMENT_TYPE_HEADER( NAME ) \
	virtual bool is##NAME() const{ return false; } \
	virtual Vulkan##NAME##Requirement* as##NAME(){ assert( false ); throw std::runtime_error( "requirement has wrong type" ); }
#define VULKAN_REQUIREMENT_TYPE_IMPL( NAME ) \
	virtual bool is##NAME() const override { return true; } \
	virtual Vulkan##NAME##Requirement* as##NAME() override { return this; }


class VulkanRequirement {
public:
	explicit VulkanRequirement();
	virtual ~VulkanRequirement();
public:
	VULKAN_REQUIREMENT_TYPE_HEADER( InstanceLayer );
	VULKAN_REQUIREMENT_TYPE_HEADER( InstanceExtension );
	VULKAN_REQUIREMENT_TYPE_HEADER( ApiVersion );
	VULKAN_REQUIREMENT_TYPE_HEADER( DeviceFeature );
	VULKAN_REQUIREMENT_TYPE_HEADER( DeviceExtension );
	VULKAN_REQUIREMENT_TYPE_HEADER( Queue );
	virtual bool isTypedDeviceExtension() const { return false; }
	virtual VulkanDeviceExtension* createDeviceExtension() const { assert( false ); }
};

class VulkanNamedRequirement : public VulkanRequirement {
private:
	const char* _name;
public:
	explicit VulkanNamedRequirement( const char* name );
	virtual ~VulkanNamedRequirement() override;
public:
	const char* name() const{ return _name; }
};

class VulkanInstanceLayerRequirement : public VulkanNamedRequirement {
public:
	explicit VulkanInstanceLayerRequirement( const char* name );
	virtual ~VulkanInstanceLayerRequirement() override;
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( InstanceLayer );
};

class VulkanInstanceExtensionRequirement : public VulkanNamedRequirement {
public:
	explicit VulkanInstanceExtensionRequirement( const char* name );
	virtual ~VulkanInstanceExtensionRequirement() override;
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( InstanceExtension );
};

class VulkanApiVersionRequirement : public VulkanRequirement {
private:
	uint32_t _apiVersion;
public:
	explicit VulkanApiVersionRequirement( uint32_t apiVersion );
	virtual ~VulkanApiVersionRequirement() override;
public:
	uint32_t apiVersion() const{ return _apiVersion; }
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( ApiVersion );
};

class VulkanDeviceFeatureRequirement : public VulkanRequirement {
private:
	VkStructureType _type;
public:
	explicit VulkanDeviceFeatureRequirement( VkStructureType type );
	virtual ~VulkanDeviceFeatureRequirement() override;
public:
	VkStructureType type() const{ return _type; }
	virtual void* untypedFeatures() = 0;
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( DeviceFeature );
};

template<typename T>
class VulkanTypedDeviceFeatureRequirement : public VulkanDeviceFeatureRequirement {
public:
	T features;
public:
	explicit VulkanTypedDeviceFeatureRequirement( VkStructureType type )
		:VulkanDeviceFeatureRequirement( type )
	{
		memset( &features, 0, sizeof( features ) );
		features.sType = type;
	}
	virtual ~VulkanTypedDeviceFeatureRequirement() override {
	}
public:
	virtual void* untypedFeatures() override { return &features; }
};

class VulkanDeviceExtensionRequirement : public VulkanNamedRequirement {
public:
	explicit VulkanDeviceExtensionRequirement( const char* name );
	virtual ~VulkanDeviceExtensionRequirement() override;
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( DeviceExtension );
};

template<typename T>
class VulkanTypedDeviceExtensionRequirement : public VulkanDeviceExtensionRequirement {
public:
	explicit VulkanTypedDeviceExtensionRequirement( const char* name )
		:VulkanDeviceExtensionRequirement( name )
	{
	}
	virtual ~VulkanTypedDeviceExtensionRequirement() override {
	}
public:
	virtual bool isTypedDeviceExtension() const override { return true; }
	virtual VulkanDeviceExtension* createDeviceExtension() const override { return new T(); }
};

class VulkanQueueRequirement : public VulkanRequirement {
public:
	explicit VulkanQueueRequirement();
	virtual ~VulkanQueueRequirement() override;
public:
	virtual VulkanQueueFamily* getQueueFamily( VulkanPhysicalDevice* physicalDevice ) const = 0;
public:
	VULKAN_REQUIREMENT_TYPE_IMPL( Queue );
};

class VulkanComputeQueueRequirement : public VulkanQueueRequirement {
public:
	explicit VulkanComputeQueueRequirement();
	virtual ~VulkanComputeQueueRequirement() override;
public:
	virtual VulkanQueueFamily* getQueueFamily( VulkanPhysicalDevice* physicalDevice ) const override;
};

class VulkanPresentQueueRequirement : public VulkanQueueRequirement {
private:
	VulkanSurface* _surface;
public:
	explicit VulkanPresentQueueRequirement( VulkanSurface* surface );
	virtual ~VulkanPresentQueueRequirement() override;
public:
	virtual VulkanQueueFamily* getQueueFamily( VulkanPhysicalDevice* physicalDevice ) const override;
};

class VulkanRequirements {
private:
	List<VulkanRequirement*> _requirements;
public:
	explicit VulkanRequirements();
	virtual ~VulkanRequirements();
public:
	const List<VulkanRequirement*>& requirements() const{ return _requirements; }
	void add( VulkanRequirement* requirement );
public:
	void requireInstanceLayer( const char* name );
	void requireInstanceExtension( const char* name );
	void requireApiVersion( uint32_t apiVersion );
public:
	void requireDeviceExtension( const char* name );
	template<typename T>
	void requireDeviceExtension(){
		add( new VulkanTypedDeviceExtensionRequirement<T>( T::Name ) );
	}
public:
	void requireComputeQueue();
	void requirePresentQueue( VulkanSurface* surface );
public:
	std::_List_const_iterator<VulkanRequirement*> begin() const {
		return _requirements.begin();
	}
	std::_List_const_iterator<VulkanRequirement*> end() const {
		return _requirements.end();
	}
};
