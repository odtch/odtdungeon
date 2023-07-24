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
	uint instance_index = globals.layers[ pld.layer_index ].first_instance_index + gl_InstanceID;
	uint material_index = instances[ instance_index ].materialIndex;
	if( 0 != ( materials[ material_index ].flags & MaterialFlag_NoShadowEmit ) ){
		ignoreIntersectionEXT;
		//debugPrintfEXT( "uuuu\n" );
	} else {
	}
	//pld.has_hit = true;
	//terminateRayEXT;
}

