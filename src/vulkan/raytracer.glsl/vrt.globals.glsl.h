#ifndef VRT_GLOBALS_H
#define VRT_GLOBALS_H

#extension GL_EXT_ray_tracing : require

layout(binding = 1, set = 0) buffer Vertices {
    Vertex vertices[];
} verticesBuffer[];

layout(binding = 2, set = 0) buffer Indices {
    uint indices[];
} indicesBuffer[];

layout( binding = 3, set = 0 ) uniform accelerationStructureEXT tlases[];

layout(binding = 8, set = 0) buffer Materials {
  VulkanMaterialData materials[];
};

layout(binding = 4, set = 0) buffer Instances {
  VulkanRenderableData instances[];
};

layout(binding = 5, set = 0) buffer Lights {
	VulkanLightData lights[];
};

layout(binding = 6) uniform sampler2D textures[];

#endif
