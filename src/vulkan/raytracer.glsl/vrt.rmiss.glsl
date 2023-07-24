#version 460
#extension GL_EXT_ray_tracing : require
//#extension GL_GOOGLE_include_directive : require

#include "vrt.common.glsl.h"

// The payload:
layout(location = 0) rayPayloadInEXT ORayPayload pld;

void main() {
  // Returns the color of the sky in a given direction (in linear color space)
  // +y in world space is up, so:
//  const float rayDirY = gl_WorldRayDirectionEXT.z;
//  if(rayDirY > 0.0f)
//  {
//    //pld.color = mix(vec3(1.0f), vec3(0.25f, 0.5f, 1.0f), rayDirY);
//      //pld.color = vec3(0.0f, 1.0f, 0.0f);
//	  //pld.color = vec3(0.2f, 0.2f, 1.0f) * 10;
//	  pld.color = vec4(0.0f, 0.10f, 0.0f, 1);
//  }
//  else
//  {
//	  pld.color = vec4(0.0f, 0.0f, 0.10f, 1);
//	//pld.color = vec3(0.3f, 0.3f, 0.3f) * 0;
//  }

//  pld.rayHitSky = true;
	pld.has_hit = false;
}
