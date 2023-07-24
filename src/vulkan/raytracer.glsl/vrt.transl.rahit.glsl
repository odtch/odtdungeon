#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable

#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "vrt.common.glsl.h"
#include "vrt.closesthit.glsl.h"
#include "vrt.ohitcalc.glsl.h"
#include "vrt.ohitcolor.glsl.h"

void main(){
	OHit hit;
	OHitCalc( hit );
	vec4 c = OHitCalcColor( hit );
	// https://developer.download.nvidia.com/video/gputechconf/gtc/2019/presentation/s91023-it-just-works-ray-traced-reflections-in-battlefield-v.pdf
	if( 0.004 < c.a ){
		pld.translucent_color += c;
	//	pld.hitColor.rgb += c.rgb * c.a;
	//	pld.hitColor.a += c.a;
		//pld.hitWeight += c.a;
	}
//	pld.hitColor.rgb += vec3( 0, 1, 0 );
//
//	pld.hitColor.a += 0.0001;
	//pld.hitColor += vec4( 0, 1, 0, 0.1 * c.a );

/*
	float w = c.a; // max( c.r, max( c.g, max( c.b, c.a ) ) );
	if( 0 < w ){
		c.rgb *= w;
		pld.hitColor += c * w;
		pld.hitWeight += w;
	}
	*/
	ignoreIntersectionEXT;
}

