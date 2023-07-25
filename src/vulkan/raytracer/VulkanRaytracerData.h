#ifndef VULKANRAYTRACERDATA
#define VULKANRAYTRACERDATA

// This file contains the Data-Structs shared by C++ and Vulkan

#include "resource/MaterialFlags.h"

#define VuklanRaytracerGlobalsData_MaxMaterialCount 80
struct VulkanMaterialData {
	uint flags;
	int texture1Index;
	uint tileCountX;
	uint tileCountY;
	vec4 color;
//	vec4 dummy_transparency;
	vec4 reflection;
//	int dummy_emissiveTextureIndex;
//	int dummy_type;
//	int dummy11;//viewport_index;
	int reserve1;
	int reserve2;
	int reserve3;
	int last;
};

#define VulkanLight_MaxCount 100
#define VulkanLightType_Ambient 1
#define VulkanLightType_Directional 2
#define VulkanLightType_Point 3
#define VulkanLightType_Spot 4
struct VulkanLightData {
	vec4 position;
	vec4 direction;
	vec4 color;
	uint type;
	float range;
	float angle;
//	uint reserve1;
//	uint reserve2;
//	uint reserve3;
//	uint reserve4;
	uint last;
};

struct VulkanRenderableData {
	vec4 color;
	uint meshIndex;
//	uint dummy_flags;
	int materialIndex;
//	int dummy_textureIndex;
//	float dummy1;//textureOffsetX;
	float textureOffsetY;
	float textureTile;
//	float dummy4;
////	vec4 datas;
////	vec4 transparency;
////	vec4 reflection;
//	int dummy_emissiveTextureIndex;
//	int dummy_viewport_index;
//	int dummy_effect;
//	int reserve_7;
//	int reserve_8;
//	int reserve_9;
	int last;
};

#define VulkanLayer_NextCameraAction_Nothing 0
#define VulkanLayer_NextCameraAction_2D_to_3D 1
#define VulkanLayer_NextCameraAction_Transform 2

#define VuklanRaytracerGlobalsData_MaxLayerCount 16
struct VulkanLayerData {
////	uint viewport_x0, viewport_x1, viewport_width, reserve01;
////	uint viewport_y0, viewport_y1, viewport_height, reserve02;
////	uint camera_projection; // 0 = perspective, 1 == Orthographic
	uint first_instance_index;
//	uint reserve31;
//	uint reserve32;
//	uint reserve33;
////	vec4 camera_position;
////	vec4 camera_direction;
////	vec4 camera_right;
////	vec4 camera_up;
////	vec4 camera_near_plane_lt;
////	vec4 camera_near_plane_rt;
////	vec4 camera_near_plane_rb;
////	vec4 camera_near_plane_lb;
	uint first_light_index;
	uint light_count;
	uint first_decal_index;
	uint decal_count;
	uint tlas_index;
	int translucent_layer_index;
//	uint reserve53;

	int next_layer_index;
	uint next_camera_action;
	float next_camera_2d_fov_size;
//	// transform im globals weil vec/mat in sub-record scheinbar nicht geht
//	uint reserve73;

////	vec4 next_camera_transform;
////	uint sky_type;
////	vec4 sky_info0;
////	vec4 sky_info1;
////	vec4 sky_info2;
//	uint reserve91;
	uint reserve92; // = 1x92;
	uint reserve93; // = 1x93;
	uint last;	    // = 1x94;
};

#define VulkanRaytracerDecal_MaxCount 256
struct VulkanRaytracerDecal {
	vec3 position;
	float radius;
	mat4 inv_transform;
	vec4 color;
	int material_index;
	float radius2;
	float textureTile;
//	//uint reserve_1_1;
//	//uint reserve_1_2;
	uint last;
};

struct VuklanRaytracerGlobalsData {
	uint target_width;
	uint target_height;
	uint target_half_width;
	uint target_half_height;

	uint sample_batch;
	uint material_count;
	uint layer_count;
	uint decal_count;
	//    uint view_mode;
	uint instance_count;
	uint mesh_count;
	uint light_count;
//    uint antialisingCount;
	uint reserve83;

	VulkanMaterialData materials[ VuklanRaytracerGlobalsData_MaxMaterialCount ];
	VulkanLayerData layers[ VuklanRaytracerGlobalsData_MaxLayerCount ];
	VulkanRaytracerDecal decals[ VulkanRaytracerDecal_MaxCount ];
//    vec4 old_view_center; // wurde verwendet, um die Anzeige auf Sichtbar von diesem Punkt aus einzuschränken
	mat4 layers_next_camera_transform[ VuklanRaytracerGlobalsData_MaxLayerCount ]; // scheinbar funktioniert vec/mat in sub-record nicht

//

	uint reserve91;
	uint reserve92; // = 1092;
	uint reserve93; // = 1093;
	uint last;	    // = 1094;
};

//#define VulkanPipelineRaytracerSensorExtension_MaxCount 10
//struct VulkanPipelineRaytracerSensorExtension_InputData {
//	vec4 position;
//	vec4 direction;
//};
//struct VulkanPipelineRaytracerSensorExtension_OutputData {
//	vec4 position;
//	vec4 normal;
//};


////const int Texture_Noise = 0;
////const int Texture_Gradient_RoundTube = 1;
////const int Texture_Gradient_SquareSphere = 2;
////const int Texture_FirstDynamic = 3;

////const uint Material_Renderable = 1;
////const uint Material_CreatesShadow = 2;
////const uint Material_CreatesLight = 4;
////const uint Material_TextureHasTransparency = 8;
////const uint Material_LightAffected = 16;
////const uint Material_AdjustColorByNormalAndCameraDirection = 32;
////const uint Material_GetColorFromNormal = 256;
////const uint Material_CalcNormalByTriangle = 512;
////const uint Material_TextCoordByPosition = 1024;
////const uint Material_Laser = 2048 * 1;
//////const uint Material_GunFireEffect = 2048 * 2;
//////const uint Material_HooverJetEffect = 2048 * 4;
//////const uint Material_BlackHole = 2048 * 8;
//////const uint Material_NormalNoise = 2048 * 16;
//////const uint Material_LaserParticle = 2048 * 32;

////const uint Viewport_Sky_Type_Constant_Color = 0;
////const uint Viewport_Sky_Type_SunSky = 1;

////const uint Light_Type_Unknown = 0;
////const uint Light_Type_Directional = 1;
////const uint Light_Type_Point = 2;
////const uint Light_Type_Spot = 3;

////const int Effect_TestTail = 1;
////const int Effect_Laser = 2;
////const int Effect_Projectile = 3;
////const int Effect_RadialPlane = 4;

////const int Camera_Projection_Perspective = 0;
////const int Camera_Projection_Orthographic = 1;


#endif
