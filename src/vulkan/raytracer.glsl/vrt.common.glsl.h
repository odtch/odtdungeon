#ifndef VRT_COMMON_H
#define VRT_COMMON_H

#include "VulkanData.h"

#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600
#define WORKGROUP_WIDTH 16
#define WORKGROUP_HEIGHT 8

layout(binding = 7, set = 0) buffer GlobalsBlock {
    VuklanRaytracerGlobalsData globals;
};

struct Vertex {
	vec3 pos;
	vec3 nrm;
	vec2 txt;
};

struct ORay {
//	vec2 pixel_coord;
//	uint depth;
	uint layer_index;
	vec3 origin;
	vec3 direction;
	//float ray_max_distance;
	//float camera_distance;
};

struct OHit {
	uint instance_index;
	uint material_index;
	uint material_flags;
	Vertex vertex[3];
	vec3 ray_direction;
	float ray_distance;
	vec3 barycentrics;
	vec3 object_position;
	vec3 object_normal;
	vec3 world_position;
	vec3 world_normal;
	vec2 texture_coord;
};

struct ORayResult {
	bool has_hit;
	vec3 hit_position;
	vec3 hit_normal;
	vec4 color;
	vec3 translucent_direction;
	vec4 translucent_factor;
	vec3 reflect_direction;
	vec4 reflect_factor;
};

struct ORayPayload {
	uint layer_index;
	bool has_hit;
	OHit hit;
	vec4 translucent_color;
};

#endif

