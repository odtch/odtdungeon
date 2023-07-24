#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_debug_printf : require
#include "vrt.common.glsl.h"
#include "vrt.closesthit.glsl.h"
#include "vrt.ohitcalc.glsl.h"

void main(){
	pld.has_hit = true;
	OHitCalc( pld.hit );
}

