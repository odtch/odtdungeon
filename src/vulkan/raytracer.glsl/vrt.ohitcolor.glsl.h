#ifndef VRT_OHITCOLOR_H
#define VRT_OHITCOLOR_H

vec4 getTileTexture( uint tile_index, vec2 texture_coord, uint tileCountX, uint tileCountY, uint texture_index ){
	//	const float border_min = 1.0 / 200;
//	const float border_max = 1.0 - border_min;
//	if( texture_coord.x < border_min || border_max < texture_coord.x ||
//		texture_coord.y < border_min || border_max < texture_coord.y
//		)
//		return vec4( 0, 1, 0, 1 );
	texture_coord.x *= 198.0 / 200;
	texture_coord.y *= 198.0 / 200;
	uint tile0X = tile_index % tileCountX;
	uint tile0Y = tile_index / tileCountX;
	texture_coord.x = ( float( tile0X ) + texture_coord.x ) / tileCountX;
	texture_coord.y = ( float( tile0Y ) + texture_coord.y ) / tileCountY;
	vec4 texture_color = texture( textures[ texture_index ], texture_coord );
	if( texture_color.a <= 0.001 ){
		return vec4( 0, 0, 0, 0 );
	} else {
		if( texture_color.a < 1 )
			texture_color.rgb *= texture_color.a; // problem: teilweise sind die rgb-komponenten trotz a = 0 gefuellt
		// texture_color.a = 1;
		return texture_color;
	}
}

vec4 OTextureGetFromMaterial( uint material_index, vec2 texture_coord, float textureTile ){
	const int texture_index = materials[ material_index ].texture1Index;
	uint tileCountX = materials[ material_index ].tileCountX;
	uint tileCountY = materials[ material_index ].tileCountY;
	if( 0 < tileCountX || 0 < tileCountY ){
		uint tileCount = tileCountX * tileCountY;
		float tileF = textureTile;
		tileF *= tileCount;
		uint tile0i = uint( tileF );
		float tile1f = tileF - tile0i;
		float tile0f = 1.0 - tile1f;
		vec4 color0 = getTileTexture( tile0i, texture_coord, tileCountX, tileCountY, texture_index );
		if( tile0i +1 < tileCount ){
			vec4 color1 = getTileTexture( tile0i + 1, texture_coord, tileCountX, tileCountY, texture_index );
	//				if( tile1f > 0.5 ){
			color0 = color0 * tile0f + color1 * tile1f;
		}
		//color0.a = tile0f;
		return color0;
	} else {
		vec4 texture_color = texture( textures[ texture_index ], texture_coord );
		if( texture_color.a <= 0 ){
			return vec4( 0, 0, 0, 0 );
		} else {
			if( texture_color.a < 1 )
				texture_color.rgb *= texture_color.a; // problem: teilweise sind die rgb-komponenten trotz a = 0 gefuellt
			return texture_color;
		}
	}
}

vec4 OHitCalcColor( in OHit hit ){
	vec4 color = vec4( 1, 1, 1, 1 );
	if( hit.material_index > globals.material_count ){
		debugPrintfEXT( "OHitCalcColor hit.material_index to high %d %d\n", hit.material_index, hit.instance_index );
		return vec4( 1, 0, 0, 1 );
	}
	if( 0 != ( hit.material_flags & MaterialFlag_UseTextCoordYasPower ) ){
		float p0 = hit.barycentrics[0] * hit.vertex[0].txt.y;
		float p1 = hit.barycentrics[1] * hit.vertex[1].txt.y;
		float p2 = hit.barycentrics[2] * hit.vertex[2].txt.y;
		if( p0 > p1 && p0 > p2 ){
			hit.texture_coord.x = hit.vertex[0].txt.x;
		} else if( p1 > p2 ){
			hit.texture_coord.x = hit.vertex[1].txt.x;
		} else {
			hit.texture_coord.x = hit.vertex[2].txt.x;
		}
		hit.texture_coord.y = 0;
	}
	if( 0 != ( hit.material_flags & MaterialFlag_UseNormalAsColor ) ){
		if( 0 != ( hit.material_flags & MaterialFlag_UseTextCoordYasPower ) ){
			float p0 = hit.barycentrics[0] * hit.vertex[0].txt.y;
			float p1 = hit.barycentrics[1] * hit.vertex[1].txt.y;
			float p2 = hit.barycentrics[2] * hit.vertex[2].txt.y;
			if( p0 > p1 && p0 > p2 ){
				color = vec4( hit.vertex[0].nrm, 1 );
			} else if( p1 > p2 ){
				color = vec4( hit.vertex[1].nrm, 1 );
			} else {
				color = vec4( hit.vertex[2].nrm, 1 );
			}
		} else {
			color = vec4( hit.vertex[0].nrm * hit.barycentrics.x + hit.vertex[1].nrm * hit.barycentrics.y + hit.vertex[2].nrm * hit.barycentrics.z, 1 );
		}
	}
	{
		// triangle-corners
//		const float minbar = 0.0031f; float maxbar = 1.0f - minbar;
//		if( hit.barycentrics[0] < minbar || hit.barycentrics[1] < minbar || hit.barycentrics[2] < minbar || hit.barycentrics[0] > maxbar || hit.barycentrics[1] > maxbar || hit.barycentrics[2] > maxbar ){
//			hit.color = vec4( 0, 0, 1, 1 );
//			if( 0 != ( hit.material_flags & MaterialFlag_UseNormalAsColor ) ){
//				hit.color = vec4( 0, 0, 1, 1 );
//			} else {
//				hit.texture_coord.x = 0;
//				hit.texture_coord.y = 0;
//			}
//		} else {
//		}
	}
	if( 0 != ( hit.material_flags & MaterialFlag_UseTextureAsMaterial ) ){
		vec3 bs = hit.barycentrics;
		bs.x *= hit.vertex[0].txt.y;
		bs.y *= hit.vertex[1].txt.y;
		bs.z *= hit.vertex[2].txt.y;
		float mf;
		if( bs.x > bs.y && bs.x > bs.z ){
			mf = hit.vertex[0].txt.x;
		} else if( bs.y > bs.z ){
			mf = hit.vertex[1].txt.x;
		} else {
			mf = hit.vertex[2].txt.x;
		}
		hit.material_index = uint( mf + 0.5f );
//		if( material_index > 10 ){
//			hit.color = vec4( 1, 0, 1, 1 );
//			return hit;
//		}
		// hack: texture-coordinaten aus mesh.vertex.position
		hit.texture_coord = hit.vertex[0].pos.xy * hit.barycentrics.x + hit.vertex[1].pos.xy * hit.barycentrics.y + hit.vertex[2].pos.xy * hit.barycentrics.z;
	}
	const int texture_index = materials[ hit.material_index ].texture1Index;



	if( 0 <= texture_index ){
		//debugPrintfEXT( "t %d %d\n", hit.material_index, texture_index );
		color *= OTextureGetFromMaterial(
					hit.material_index,
					hit.texture_coord,
					instances[ hit.instance_index ].textureTile
					);
		//debugPrintfEXT( "vrt.t %d %f,%f \n", texture_index, hit.texture_coord.x * 512, hit.texture_coord.y * 512 );
		//debugPrintfEXT( "vrt.t %d %f,%f is %f %f %f %f\n", texture_index, hit.texture_coord.x * 512, hit.texture_coord.y * 512, texture_color.r * 256, texture_color.g * 256, texture_color.b * 256, texture_color.a * 256 );
		//return vec4( 0, 1, 0, 1 );
	} else {
		//return vec4( 0, 0, 1, 1 );

	}
	color *= materials[ hit.material_index ].color;
	color *= instances[ hit.instance_index ].color;
	if( 0 != ( hit.material_flags & MaterialFlag_ViewOrentationToAlpha ) ){
		float f =  abs( dot( hit.world_normal, hit.ray_direction ) );
		if( f < 0.4 ){
			color.a = 0;
		} else if( f < 0.6 ){
			color.a *= ( f - 0.4 ) / 0.2;
		} else {
			//hit.color.a *= 1;
		}
	}
	if( color.a < 0 )color.a = 0;
	if( color.a > 1 )color.a = 1;
	return color;
}


#endif

