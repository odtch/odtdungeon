#ifndef VRT_ORAY_H
#define VRT_ORAY_H

#include "vrt.ohitcolor.glsl.h"
#include "vrt.otransl.glsl.h"
#include "vrt.olight.glsl.h"
#include "vrt.decal.glsl.h"

ORayResult ORayCalc( in ORay ray ){
	ORayResult result;
	pld.layer_index = ray.layer_index;
	pld.has_hit = false;
	traceRayEXT(
		tlases[ globals.layers[ ray.layer_index ].tlas_index ],
		gl_RayFlagsOpaqueEXT,  // Ray flags, here saying "treat all geometry as opaque"
		0xFF,                  // 8-bit instance mask, here saying "trace against all instances"
		0,                     // SBT record offset
		0,                     // SBT record stride for offset
		0,                     // Miss index
		ray.origin,
		0.0, // Minimum t-value
		ray.direction,
		10000, // ray_max_length, // Maximum t-value
		0 // Location of payload
	);
	if( pld.has_hit ){
		result.has_hit = true;
		result.hit_position = pld.hit.world_position;
		result.hit_normal = pld.hit.world_normal;
		result.color = OHitCalcColor( pld.hit );
		vec4 decal = ODecalCalc( ray.layer_index, pld.hit );
		if( 0 < decal.a ){
			result.color.rgb *= ( 1.0 - decal.a );
			result.color.rgb += decal.rgb;
		}
		result.translucent_factor = vec4( 1.0 - result.color.a );
		result.translucent_direction = ray.direction;
		result.reflect_factor = materials[ pld.hit.material_index ].reflection;
		if( 0 < result.reflect_factor.a ){
			result.reflect_direction = reflect( ray.direction, pld.hit.world_normal );
		}
		if( 0 != ( materials[ pld.hit.material_index ].flags & MaterialFlag_NoLightAffected ) ){
		} else {
			vec4 light_color = OCalcLights( ray.layer_index, result.hit_position, result.hit_normal );
			result.color.rgb *= light_color.rgb;
		}
result.color = vec4( 0, 0, 1, 1 );
	} else {
		result.has_hit = false;
		result.color = vec4( 0 );
		result.hit_position = vec3( 0 );
		result.translucent_direction = vec3( 0 );
		result.translucent_factor = vec4( 0 );
		result.reflect_direction = vec3( 0 );
		result.reflect_factor = vec4( 0 );
	}
	/**/
	if( 0 <= globals.layers[ ray.layer_index ].translucent_layer_index ){
		float translucent_max_length;
		if( result.has_hit ){
			translucent_max_length = length( result.hit_position - ray.origin );
		} else {
			translucent_max_length = 9999999;
		}
		vec4 translucent_color = OCalcTranslucent( globals.layers[ ray.layer_index ].translucent_layer_index, ray.origin, ray.direction, translucent_max_length );
		if( translucent_color.a > 0 ){
			result.color.rgb *= ( 1.0 - translucent_color.a );
			result.color.rgb += translucent_color.rgb * translucent_color.a;
//			if( result.color.a < translucent_color.a )
//				result.color.a = translucent_color.a;
		}
	}
	/**/
	return result;
}

struct ORayStack {
	uint index;
	ORay ray;
	vec4 color;
	vec4 dest_factor;
	uint dest_index;
};

bool ORayNextLayer( vec2 pixel, uint layer_index, out ORay ray ){
	if( globals.layers[ layer_index ].next_layer_index < 0 ){
		return false;
	} else if( globals.layers[ layer_index ].next_camera_action == VulkanLayer_NextCameraAction_2D_to_3D ){
		ray.direction = vec3( -( globals.target_half_width - pixel.x ), globals.layers[ layer_index ].next_camera_2d_fov_size, ( globals.target_half_height - pixel.y ) );
		ray.direction = normalize( ray.direction );
		ray.origin = vec3( 0, 0, 0 );
		ray.origin = ( globals.layers_next_camera_transform[ layer_index ] * vec4( ray.origin, 1 ) ).xyz;
		ray.direction = ( globals.layers_next_camera_transform[ layer_index ] * vec4( ray.direction, 0 ) ).xyz;
		ray.direction = normalize( ray.direction );
	} else if( globals.layers[ layer_index ].next_camera_action == VulkanLayer_NextCameraAction_Transform ){
		ray.origin = ( globals.layers_next_camera_transform[ layer_index ] * vec4( ray.origin, 1 ) ).xyz;
		ray.direction = ( globals.layers_next_camera_transform[ layer_index ] * vec4( ray.direction, 0 ) ).xyz;
		ray.direction = normalize( ray.direction );
	} else {
		debugPrintfEXT( "ORayNextLayer unknown action %d\n", layer_index );
		return false;
	}
	ray.layer_index = globals.layers[ layer_index ].next_layer_index;
	return true;
}


vec4 ORayResolve( vec2 pixel, uint start_layer_index, vec3 start_ray_origin, vec3 start_ray_direction ){
	const uint ORAY_STACKSIZE = 6;
	ORayStack stack[ ORAY_STACKSIZE ];
	uint stack_count = 0;
	stack[stack_count].index = stack_count;
	stack[stack_count].ray.layer_index = start_layer_index;
	stack[stack_count].ray.origin = start_ray_origin;
	stack[stack_count].ray.direction = start_ray_direction;
	stack[stack_count].dest_factor = vec4( 1 );
	stack[stack_count].dest_index = 0;
	stack_count++;
	{
		for( uint s = 0; s < stack_count; s++ ){
			ORayResult result = ORayCalc( stack[s].ray );
			if( result.has_hit ){
				//return result.color;
				/**/
				stack[s].color = result.color;
				if( 0 < result.translucent_factor.a && stack_count < ORAY_STACKSIZE ){
					stack[stack_count].index = stack_count;
					stack[stack_count].ray.layer_index = stack[s].ray.layer_index;
					stack[stack_count].ray.origin = offsetPositionAlongNormal( result.hit_position, result.translucent_direction );
					stack[stack_count].ray.direction = result.translucent_direction;
					stack[stack_count].dest_factor = result.translucent_factor;
					stack[stack_count].dest_index = s;
					stack_count++;
					//if( s == 1 )return vec4( 1, 0, 0, 1 );
				}
				if( 0 < result.reflect_factor.a && stack_count < ORAY_STACKSIZE ){
					stack[stack_count].index = stack_count;
					stack[stack_count].ray.layer_index = stack[s].ray.layer_index;
					stack[stack_count].ray.origin = offsetPositionAlongNormal( result.hit_position, result.reflect_direction );
					stack[stack_count].ray.direction = result.reflect_direction;
					stack[stack_count].dest_factor = result.reflect_factor;
					stack[stack_count].dest_index = s;
					stack_count++;
					//if( s == 1 )return vec4( 0, 1, 0, 1 );
				}
				/**/
			} else {
				//if( s == 2 )return vec4( 0, 1, 0, 1 );
				stack[s].color = vec4( 0 );
				if( stack_count + 1 < ORAY_STACKSIZE ){
					if( ORayNextLayer( pixel, stack[s].ray.layer_index, stack[stack_count].ray ) ){
						stack[stack_count].index = stack_count;
						stack[stack_count].dest_factor = stack[s].dest_factor;
						stack[stack_count].dest_index = stack[s].dest_index;
						stack[s].dest_index = 999;
						stack_count++;
					} else {
						stack[s].color = vec4( 0, 0, 0, 0 );
					}
				} else {
					stack[s].color = vec4( 0, 0, 0, 0 );
				}
			}
		}
	}{
		for( uint s = stack_count -1; 0 < s; s-- ){
			if( 998 < stack[s].dest_index ){
			} else {
				stack[ stack[s].dest_index ].color += stack[s].color * stack[s].dest_factor;
			}
		}
	}
	return stack[ 0 ].color;
}

/*
	if( 0 != ( materials[ material_index ].flags & MaterialFlag_Volumetric ) ){
//		vec4 pc = hitInfo.color;
//		vec4 light_color = calcLightsColor( hitInfo.worldPosition, hitInfo.worldNormal );
//		//pld.hitColor *= light_color;
//		//pld.hitColor = pc * 0.5f + pc * light_color ;
//		hitInfo.color.rgb = ( pc * light_color ).rgb;
//		hitInfo.color.a = pc.a;
//		hitInfo.color = vec4( 0, 1, 0, 1);
//		hitInfo.color.a = 0.1f;
		if( abs( dot( hitInfo.worldNormal, gl_WorldRayDirectionEXT ) ) < 0.03 ){
			hitInfo.color = vec4( 0, 0, 0, 1 );
		} else {
			float vl = 0;
			if( pld.depth < 2 ){
				pld.depth++;
				pld.hasHit = false;
				traceRayEXT(
						  tlases[ globals.layers[ pld.layer ].tlas_index ],
						  gl_RayFlagsOpaqueEXT,  // Ray flags, here saying "treat all geometry as opaque"
						  0xFF,                  // 8-bit instance mask, here saying "trace against all instances"
						  0,                     // SBT record offset
						  0,                     // SBT record stride for offset
						  0,                     // Miss index
						  hitInfo.worldPosition
							//+ hitInfo.worldNormal * -0.000001
							- hitInfo.worldNormal * 0.0001
							//+ gl_WorldRayDirectionEXT * 0.001
						,
						  0.0,                   // Minimum t-value
						  gl_WorldRayDirectionEXT,
						  40.0,               // Maximum t-value
						  0);                    // Location of payload
				pld.depth--;
				if( pld.hasHit ){
					vl = length( pld.hitPosition - hitInfo.worldPosition );
					//hitInfo.color += pld.hitColor;
					if( pld.instance_index == hitInfo.instance_index ){
						hitInfo.worldPosition = pld.hitPosition + gl_WorldRayDirectionEXT * 0.001;
					} else {
						//vl = 0;
					}
				} else {
					vl = 9999999;
					vl = 0;
				}
			} else {
				vl = 9999999;
				vl = 0;
			}
			vl *= hitInfo.color.a;
			if( vl > 1 ){
				vl = 1;
			}
			hitInfo.color *= vl;
			hitInfo.color.a = vl;
//			if( vl > 0.75f ){
//				vl = 1;
//				hitInfo.color = vec4( 0.3, 0, 0, 0 );
//			} else {
//				hitInfo.color = vec4( 0, 0, 0, 1 );

//			}

//			if( pld.depth == 0 ){
//				hitInfo.color = vec4( 0, 1, 0, 0 );
//			} else if( pld.depth == 1 ){
//				hitInfo.color = vec4( 0, 0, 1, 0 );
//			} else {
//				hitInfo.color = vec4( 1, 0, 0, 0 );
//			}
//			hitInfo.color = vec4( 1, 1, 1, 1 ) * vl;
//			//hitInfo.color.a = 1;
//			hitInfo.color.a = vl;
//			hitInfo.color.r = 1;

//			vl *= vl;
//			vl /= 14;
//			if( vl > 1 ){
//				vl = 1;
//			}
//			hitInfo.worldPosition += gl_WorldRayDirectionEXT * vl * 1.01f;
//			hitInfo.color *= vl;
//			hitInfo.color.a = 1.0 - vl;
		}
		//hitInfo.color.a = 0;
		//hitInfo.color.a = 1;
	}

 */
#endif

