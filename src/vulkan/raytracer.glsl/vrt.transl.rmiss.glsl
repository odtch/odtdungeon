#version 460
#extension GL_EXT_ray_tracing : require
//#extension GL_GOOGLE_include_directive : require

#include "vrt.common.glsl.h"

layout(location = 0) rayPayloadInEXT ORayPayload pld;

void main() {
	pld.has_hit = false;
}
