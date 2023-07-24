#ifndef VRT_OTRANSL_H
#define VRT_OTRANSL_H

vec4 OCalcTranslucent( uint layer_index, vec3 camera_position, vec3 camera_direction, float translucent_max_length ){
	pld.layer_index = layer_index;
	pld.has_hit = false;
	pld.translucent_color = vec4( 0 );
	traceRayEXT(
		tlases[ globals.layers[ layer_index ].tlas_index ],
		gl_RayFlagsNoneEXT,
		0xFF,                  // 8-bit instance mask, here saying "trace against all instances"
		2,                     // SBT record offset
		0,                     // SBT record stride for offset
		2,                     // Miss index
		camera_position,
		0.0, // Minimum t-value
		camera_direction,
		translucent_max_length, // Maximum t-value
		0 // Location of payload
	);
	vec4 r = pld.translucent_color;
	if( r.a < 1 ){
		//return vec4( 0, 1, 0, 1 );
		//r.rgb *= r.a;
	} else {
		//return vec4( 1, 0, 0, 1 );
		r.rgb /= r.a;
		r.a = 1.0;
		//r /= r.a;
		//return vec4( 0, 1, 0, 1 );
	}
	return r;
}

#endif
