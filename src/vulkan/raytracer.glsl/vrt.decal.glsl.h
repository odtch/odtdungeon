#include "vrt.utils.glsl.h"

vec4 ODecalCalc( uint layer_index, in OHit hit ){
	uint di = globals.layers[ layer_index ].first_decal_index;
	vec4 color = vec4( 0 );
	for( uint dc = 0; dc < globals.layers[ layer_index ].decal_count; dc++ ){
		vec3 offset = ( globals.decals[ di ].position - hit.world_position );
		float dist2 = length2( offset );
		//dist2 *= 0.000001;
		if( dist2 <= globals.decals[ di ].radius2 ){
			vec4 t = globals.decals[ di ].inv_transform * vec4( offset, 0 );
			t /= globals.decals[ di ].radius * 0.71;
			if( t.x < -1 || 1 < t.x ){
				//return vec4( 1, 0, 0, 1 );
			} else  if( t.y < -1 || 1 < t.y ){
				//return vec4( 0, 1, 0, 1 );
			} else {
				//color *= 0.01;
				//vec4 color;
				vec2 texture_coord = vec2( ( 1 + t.x ) / 2, ( 1 + t.y ) / 2 );
				//color.r = texture_coord.x;
				//color.g = texture_coord.y;
				//color = vec4( texture_coord.y, 0, 0, 1 );
//				if( 0 <= globals.decals[ di ].texture_index ){
//					color += texture( textures[ globals.decals[ di ].texture_index ], texture_coord );
////					color = vec4( 0, 1, 1, 1 );
//				} else {
////					color = vec4( 1, 0, 0, 1 );
//				}
				color += OTextureGetFromMaterial( globals.decals[ di ].material_index, texture_coord, globals.decals[ di ].textureTile )
						* globals.decals[ di ].color;

				//color.a = 1;
//				color.r = texture_coord.x;
//				color.g = texture_coord.y;
//				color.r = globals.decals[ di ].textureTile;
//				color.g = globals.decals[ di ].textureTile;
//				color.b = globals.decals[ di ].textureTile;

				//hit.color.b = 1;
				//hit.color.g = 1;
			}
		}
//		float d = length( offset );
//		if( d < 10 ){
//			hit.color.g = 1;
//		} else {
//			hit.color.b = 1;
//		}
		di++;
	}
	if( color.a > 1 ){
		color /= color.a;
	}
//
//	hit.color.g = 1;
	return color;
}

