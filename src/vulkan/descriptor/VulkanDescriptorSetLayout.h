#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/device/VulkanDeviceObject.h"

class VulkanDescriptorSetLayout : public VulkanDeviceObject
{
private:
	enum { Max_Binding_Count = 11 };
	int _bindings_count;
	VkDescriptorSetLayoutBinding _bindings[ Max_Binding_Count ];
private:
	VkDescriptorSetLayout _vkDescriptorSetLayout;
public:
	explicit VulkanDescriptorSetLayout();
	~VulkanDescriptorSetLayout();
public:
	void addBinding( uint32_t binding, VkDescriptorType type,  uint32_t count, VkShaderStageFlags stageFlags, const VkSampler* pImmutableSampler = nullptr );
	int bindingCount() const{ return _bindings_count; }
	const VkDescriptorType& descriptorType( int i ) const;
	uint32_t descriptorSize( int i ) const;
public:
	void create( VulkanDevice* device );
	bool isCreated() const{ return _vkDescriptorSetLayout != null; }
	void destroy();
//public:
//	void waitFor( uint64_t timeout_ns = 1000 * 1000 * 1000 );
//	void reset();
public:
	const VkDescriptorSetLayout& vkDescriptorSetLayout() const{ assert( _vkDescriptorSetLayout ); return _vkDescriptorSetLayout; }
};


/*
  # class nvvk::DescriptorSetBindings

  Helper class that keeps a vector of `VkDescriptorSetLayoutBinding` for a single
  `VkDescriptorSetLayout`. Provides helper functions to create `VkDescriptorSetLayout`
  as well as `VkDescriptorPool` based on this information, as well as utilities
  to fill the `VkWriteDescriptorSet` structure with binding information stored
  within the class.

  The class comes with the convenience functionality that when you make a
  VkWriteDescriptorSet you provide the binding slot, rather than the
  index of the binding's storage within this class. This results in a small
  linear search, but makes it easy to change the content/order of bindings
  at creation time.

  Example :
  ~~~C++
  DescriptorSetBindings binds;

  binds.addBinding( VIEW_BINDING, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
  binds.addBinding(XFORM_BINDING, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);

  VkDescriptorSetLayout layout = binds.createLayout(device);

  #if SINGLE_LAYOUT_POOL
	// let's create a pool with 2 sets
	VkDescriptorPool      pool   = binds.createPool(device, 2);
  #else
	// if you want to combine multiple layouts into a common pool
	std::vector<VkDescriptorPoolSize> poolSizes;
	bindsA.addRequiredPoolSizes(poolSizes, numSetsA);
	bindsB.addRequiredPoolSizes(poolSizes, numSetsB);
	VkDescriptorPool      pool   = nvvk::createDescriptorPool(device, poolSizes,
															  numSetsA + numSetsB);
  #endif

  // fill them
  std::vector<VkWriteDescriptorSet> updates;

  updates.push_back(binds.makeWrite(0, VIEW_BINDING, &view0BufferInfo));
  updates.push_back(binds.makeWrite(1, VIEW_BINDING, &view1BufferInfo));
  updates.push_back(binds.makeWrite(0, XFORM_BINDING, &xform0BufferInfo));
  updates.push_back(binds.makeWrite(1, XFORM_BINDING, &xform1BufferInfo));

  vkUpdateDescriptorSets(device, updates.size(), updates.data(), 0, nullptr);
  ~~~
* /

class DescriptorSetBindings
{
public:
  DescriptorSetBindings() = default;
  DescriptorSetBindings(const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	  : m_bindings(bindings)
  {
  }

  // Add a binding to the descriptor set
  void addBinding(uint32_t binding,          // Slot to which the descriptor will be bound, corresponding to the layout
											 // binding index in the shader
				  VkDescriptorType   type,   // Type of the bound descriptor(s)
				  uint32_t           count,  // Number of descriptors
				  VkShaderStageFlags stageFlags,  // Shader stages at which the bound resources will be available
				  const VkSampler*   pImmutableSampler = nullptr  // Corresponding sampler, in case of textures
  )
  {
	m_bindings.push_back({binding, type, count, stageFlags, pImmutableSampler});
  }

  void addBinding(const VkDescriptorSetLayoutBinding& layoutBinding) { m_bindings.emplace_back(layoutBinding); }

  void setBindings(const std::vector<VkDescriptorSetLayoutBinding>& bindings) { m_bindings = bindings; }

  // requires use of SUPPORT_INDEXING_EXT/SUPPORT_INDEXING_V1_2 on createLayout
  void setBindingFlags(uint32_t binding, VkDescriptorBindingFlags bindingFlags);

  void clear()
  {
	m_bindings.clear();
	m_bindingFlags.clear();
  }
  bool                                empty() const { return m_bindings.empty(); }
  size_t                              size() const { return m_bindings.size(); }
  const VkDescriptorSetLayoutBinding* data() const { return m_bindings.data(); }

  VkDescriptorType getType(uint32_t binding) const;
  uint32_t         getCount(uint32_t binding) const;


  // Once the bindings have been added, this generates the descriptor layout corresponding to the
  // bound resources.
  VkDescriptorSetLayout createLayout(VkDevice                         device,
									 VkDescriptorSetLayoutCreateFlags flags        = 0,
									 DescriptorSupport                supportFlags = DescriptorSupport::CORE_1_0) const;

  // Once the bindings have been added, this generates the descriptor pool with enough space to
  // handle all the bound resources and allocate up to maxSets descriptor sets
  VkDescriptorPool createPool(VkDevice device, uint32_t maxSets = 1) const;

  // appends the required poolsizes for N sets
  void addRequiredPoolSizes(std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t numSets) const;

  // provide single element
  VkWriteDescriptorSet makeWrite(VkDescriptorSet dstSet, uint32_t dstBinding, uint32_t arrayElement = 0) const;
  VkWriteDescriptorSet makeWrite(VkDescriptorSet              dstSet,
								 uint32_t                     dstBinding,
								 const VkDescriptorImageInfo* pImageInfo,
								 uint32_t                     arrayElement = 0) const;
  VkWriteDescriptorSet makeWrite(VkDescriptorSet               dstSet,
								 uint32_t                      dstBinding,
								 const VkDescriptorBufferInfo* pBufferInfo,
								 uint32_t                      arrayElement = 0) const;
  VkWriteDescriptorSet makeWrite(VkDescriptorSet     dstSet,
								 uint32_t            dstBinding,
								 const VkBufferView* pTexelBufferView,
								 uint32_t            arrayElement = 0) const;
#if VK_NV_ray_tracing
  VkWriteDescriptorSet makeWrite(VkDescriptorSet                                    dstSet,
								 uint32_t                                           dstBinding,
								 const VkWriteDescriptorSetAccelerationStructureNV* pAccel,
								 uint32_t                                           arrayElement = 0) const;
#endif
#if VK_EXT_inline_uniform_block
  VkWriteDescriptorSet makeWrite(VkDescriptorSet                                  dstSet,
								 uint32_t                                         dstBinding,
								 const VkWriteDescriptorSetInlineUniformBlockEXT* pInlineUniform,
								 uint32_t                                         arrayElement = 0) const;
#endif
  // provide full array
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet dstSet, uint32_t dstBinding) const;
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorImageInfo* pImageInfo) const;
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet dstSet, uint32_t dstBinding, const VkDescriptorBufferInfo* pBufferInfo) const;
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet dstSet, uint32_t dstBinding, const VkBufferView* pTexelBufferView) const;
#if VK_NV_ray_tracing
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet                                    dstSet,
									  uint32_t                                           dstBinding,
									  const VkWriteDescriptorSetAccelerationStructureNV* pAccel) const;
#endif
#if VK_EXT_inline_uniform_block
  VkWriteDescriptorSet makeWriteArray(VkDescriptorSet                                  dstSet,
									  uint32_t                                         dstBinding,
									  const VkWriteDescriptorSetInlineUniformBlockEXT* pInline) const;
#endif
#ifdef VULKAN_HPP
  void addBinding(uint32_t binding,           // Slot to which the descriptor will be bound, corresponding to the layout
											  // binding index in the shader
				  vk::DescriptorType   type,  // Type of the bound descriptor(s)
				  uint32_t             count,       // Number of descriptors
				  vk::ShaderStageFlags stageFlags,  // Shader stages at which the bound resources will be available
				  const vk::Sampler*   pImmutableSampler = nullptr  // Corresponding sampler, in case of textures
  )
  {
	m_bindings.push_back({binding, static_cast<VkDescriptorType>(type), count, static_cast<VkShaderStageFlags>(stageFlags),
						  reinterpret_cast<const VkSampler*>(pImmutableSampler)});
  }
  void setBindings(const std::vector<vk::DescriptorSetLayoutBinding>& bindings)
  {
	setBindings(reinterpret_cast<const std::vector<VkDescriptorSetLayoutBinding>&>(bindings));
  }

  void setBindingFlags(uint32_t binding, vk::DescriptorBindingFlags bindingFlags)
  {
	setBindingFlags(binding, static_cast<VkDescriptorBindingFlags>(bindingFlags));
  }

  void addRequiredPoolSizes(std::vector<vk::DescriptorPoolSize>& poolSizes, uint32_t numSets) const
  {
	addRequiredPoolSizes(reinterpret_cast<std::vector<VkDescriptorPoolSize>&>(poolSizes), numSets);
  }

  vk::WriteDescriptorSet makeWrite(vk::DescriptorSet              dstSet,
								   uint32_t                       dstBinding,
								   const vk::DescriptorImageInfo* pImageInfo,
								   uint32_t                       arrayElement = 0) const
  {
	return makeWrite(dstSet, dstBinding, reinterpret_cast<const VkDescriptorImageInfo*>(pImageInfo), arrayElement);
  }
  vk::WriteDescriptorSet makeWrite(vk::DescriptorSet               dstSet,
								   uint32_t                        dstBinding,
								   const vk::DescriptorBufferInfo* pBufferInfo,
								   uint32_t                        arrayElement = 0) const
  {
	return makeWrite(dstSet, dstBinding, reinterpret_cast<const VkDescriptorBufferInfo*>(pBufferInfo), arrayElement);
  }
  vk::WriteDescriptorSet makeWrite(vk::DescriptorSet     dstSet,
								   uint32_t              dstBinding,
								   const vk::BufferView* pTexelBufferView,
								   uint32_t              arrayElement = 0) const
  {
	return makeWrite(dstSet, dstBinding, reinterpret_cast<const VkBufferView*>(pTexelBufferView), arrayElement);
  }
#if VK_NV_ray_tracing
  vk::WriteDescriptorSet makeWrite(vk::DescriptorSet                                    dstSet,
								   uint32_t                                             dstBinding,
								   const vk::WriteDescriptorSetAccelerationStructureNV* pAccel,
								   uint32_t                                             arrayElement = 0) const
  {
	return makeWrite(dstSet, dstBinding, reinterpret_cast<const VkWriteDescriptorSetAccelerationStructureNV*>(pAccel), arrayElement);
  }
#endif
#if VK_EXT_inline_uniform_block
  vk::WriteDescriptorSet makeWrite(vk::DescriptorSet                                  dstSet,
								   uint32_t                                           dstBinding,
								   const vk::WriteDescriptorSetInlineUniformBlockEXT* pInlineUniform,
								   uint32_t                                           arrayElement = 0) const
  {
	return makeWrite(dstSet, dstBinding,
					 reinterpret_cast<const VkWriteDescriptorSetInlineUniformBlockEXT*>(pInlineUniform), arrayElement);
  }
#endif
  vk::WriteDescriptorSet makeWriteArray(vk::DescriptorSet dstSet, uint32_t dstBinding, const vk::DescriptorImageInfo* pImageInfo) const
  {
	return makeWriteArray(dstSet, dstBinding, reinterpret_cast<const VkDescriptorImageInfo*>(pImageInfo));
  }
  vk::WriteDescriptorSet makeWriteArray(vk::DescriptorSet dstSet, uint32_t dstBinding, const vk::DescriptorBufferInfo* pBufferInfo) const
  {
	return makeWriteArray(dstSet, dstBinding, reinterpret_cast<const VkDescriptorBufferInfo*>(pBufferInfo));
  }
  vk::WriteDescriptorSet makeWriteArray(vk::DescriptorSet dstSet, uint32_t dstBinding, const vk::BufferView* pTexelBufferView) const
  {
	return makeWriteArray(dstSet, dstBinding, reinterpret_cast<const VkBufferView*>(pTexelBufferView));
  }
#if VK_NV_ray_tracing
  vk::WriteDescriptorSet makeWriteArray(vk::DescriptorSet                                    dstSet,
										uint32_t                                             dstBinding,
										const vk::WriteDescriptorSetAccelerationStructureNV* pAccel) const
  {
	return makeWriteArray(dstSet, dstBinding, reinterpret_cast<const VkWriteDescriptorSetAccelerationStructureNV*>(pAccel));
  }
#endif
#if VK_EXT_inline_uniform_block
  vk::WriteDescriptorSet makeWriteArray(vk::DescriptorSet                                  dstSet,
										uint32_t                                           dstBinding,
										const vk::WriteDescriptorSetInlineUniformBlockEXT* pInline) const
  {
	return makeWriteArray(dstSet, dstBinding, reinterpret_cast<const VkWriteDescriptorSetInlineUniformBlockEXT*>(pInline));
  }
#endif
#endif

private:
  std::vector<VkDescriptorSetLayoutBinding> m_bindings;
  std::vector<VkDescriptorBindingFlags>     m_bindingFlags;
};
 * */
