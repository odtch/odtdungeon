#ifndef VRT_OLIGHT_H
#define VRT_OLIGHT_H

#include "vrt.utils.glsl.h"

vec4 OCalcLight( uint layer_index, uint light_index, vec3 position, vec3 normal ){
	if( lights[ light_index ].type == VulkanLightType_Ambient ){
		return lights[ light_index ].color;
	}
	vec3 light_direction;
	float light_factor;
	float light_range;
	if( lights[ light_index ].type == VulkanLightType_Directional ){
		light_direction = lights[ light_index ].direction.xyz;
		light_factor = 1;
		light_range = 9999999;
	} else {
		vec3 light_offset = position - lights[ light_index ].position.xyz;
		float light_dist = length( light_offset );
		if( light_dist <= 0.0001 )
			return lights[ light_index ].color;
		if( lights[ light_index ].range < light_dist )
			return vec4( 0 );
		light_range = light_dist;//lights[ light_index ].range - light_dist;
		light_factor = 1.0f - ( light_dist / lights[ light_index ].range );
		light_direction = light_offset / light_dist;
		if( lights[ light_index ].type == VulkanLightType_Point ){
		//} else if( lights[ light_index ].type == VulkanLightType_Spot ){
		} else {
			debugPrintfEXT( "calcLightColor unhandled type %d in %d\n", lights[ light_index ].type, light_index );
			return vec4( 1, 0, 0, 0 );
		}
	}
	float dot_normal_light_direction = dot( normal, -light_direction );
	if( dot_normal_light_direction <= 0 )
		return vec4( 0 );
	vec3 np = offsetPositionAlongNormal( position, normal );
	pld.layer_index = layer_index;
	pld.has_hit = false;
	traceRayEXT(
		tlases[ globals.layers[ layer_index ].tlas_index ],
		gl_RayFlagsNoneEXT, // gl_RayFlagsOpaqueEXT, // gl_RayFlagsNoneEXT,  // rayFlags
		0xFF,               // cullMask
		1,                  // sbtRecordOffset
		0,                  // sbtRecordStride
		1,                  // missIndex
		np,
		0,                // ray min range
		- light_direction,
		light_range, // ray max range
		0                   // payload (location = 0)
	);
	if( pld.has_hit )
		return vec4( 0 );
	return lights[ light_index ].color * dot_normal_light_direction;// * light_factor;
}

vec4 OCalcLights( uint layer_index, vec3 position, vec3 normal ){
	vec4 light_color = vec4( 0 );
	for( uint l = 0; l < globals.layers[ layer_index ].light_count; l++ ){
		uint light_index = globals.layers[ layer_index ].first_light_index + l;
		light_color += OCalcLight( layer_index, light_index, position, normal );
//		light_color[ l ] += 0.5f;
	}
	return light_color;
}

#endif
