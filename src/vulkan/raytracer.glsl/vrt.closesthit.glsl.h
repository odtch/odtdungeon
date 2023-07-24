#ifndef VRT_CLOSESTHIT_H
#define VRT_CLOSESTHIT_H

#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_nonuniform_qualifier : require
//#extension GL_EXT_gpu_shader4 : enable // for bit-wise-operators
#include "vrt.common.glsl.h"
#include "vrt.globals.glsl.h"

#extension GL_EXT_debug_printf : require

// This will store two of the barycentric coordinates of the intersection when
// closest-hit shaders are called:
hitAttributeEXT vec2 attributes;

layout(location = 0) rayPayloadInEXT ORayPayload pld;

//#include "vrt.ohit.glsl.h"

#endif
