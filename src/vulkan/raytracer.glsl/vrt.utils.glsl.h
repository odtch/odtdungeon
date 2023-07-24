#ifndef VRT_UTILS_H
#define VRT_UTILS_H

float length2( vec3 v ){
	return ( v.x * v.x ) + ( v.y * v.y ) + ( v.z * v.z );
}

/*
struct VulkanRayInfoOOOOOLD
{
	uint layer;
  vec3 rayOrigin;     // The new ray origin in world-space.
  vec3 rayDirection;  // The new ray direction in world-space.
  uint depth;
  bool hasHit;
  vec3	hitPosition;
  vec4 hitColor;         // The reflectivity of the surface.
  float hitWeight; // used by translucent-layer
  uint instance_index;
  uint material_index;
  uint rngState;      // State of the random number generator.
  //uint anyhitcount;
};

// Steps the RNG and returns a floating-point value between 0 and 1 inclusive.
float stepAndOutputRNGFloat(inout uint rngState)
{
  // Condensed version of pcg_output_rxs_m_xs_32_32, with simple conversion to floating-point [0,1].
  rngState  = rngState * 747796405 + 1;
  uint word = ((rngState >> ((rngState >> 28) + 4)) ^ rngState) * 277803737;
  word      = (word >> 22) ^ word;
  return float(word) / 4294967295.0f;
}

const float k_pi = 3.14159265;


// Uses the Box-Muller transform to return a normally distributed (centered
// at 0, standard deviation 1) 2D point.
vec2 randomGaussian(inout uint rngState)
{
  // Almost uniform in (0,1] - make sure the value is never 0:
  const float u1    = max(1e-38, stepAndOutputRNGFloat(rngState));
  const float u2    = stepAndOutputRNGFloat(rngState);  // In [0, 2pi]
  const float r     = sqrt(-2.0 * log(u1));
  const float theta = 2 * k_pi * u2;  // Random in [0, 2pi]
  return r * vec2(cos(theta), sin(theta));
}

vec3 diffuseReflection(vec3 normal, inout uint rngState)
{
  // For a random diffuse bounce direction, we follow the approach of
  // Ray Tracing in One Weekend, and generate a random point on a sphere
  // of radius 1 centered at the normal. This uses the random_unit_vector
  // function from chapter 8.5:
  const float theta     = 2.0 * k_pi * stepAndOutputRNGFloat(rngState);  // Random in [0, 2pi]
  const float u         = 2.0 * stepAndOutputRNGFloat(rngState) - 1.0;   // Random in [-1, 1]
  const float r         = sqrt(1.0 - u * u);
  const vec3  direction = normal + vec3(r * cos(theta), r * sin(theta), u);

  // Then normalize the ray direction:
  return normalize(direction);
}


*/

// offsetPositionAlongNormal shifts a point on a triangle surface so that a
// ray bouncing off the surface with tMin = 0.0 is no longer treated as
// intersecting the surface it originated from.
//
// Here's the old implementation of it we used in earlier chapters:
// vec3 offsetPositionAlongNormal(vec3 worldPosition, vec3 normal)
// {
//   return worldPosition + 0.0001 * normal;
// }
//
// However, this code uses an improved technique by Carsten Wächter and
// Nikolaus Binder from "A Fast and Robust Method for Avoiding
// Self-Intersection" from Ray Tracing Gems (verion 1.7, 2020).
// The normal can be negated if one wants the ray to pass through
// the surface instead.
vec3 offsetPositionAlongNormal( vec3 worldPosition, vec3 normal ){
  // Convert the normal to an integer offset.
  const float int_scale = 256.0f;
  const ivec3 of_i      = ivec3(int_scale * normal);
  // Offset each component of worldPosition using its binary representation.
  // Handle the sign bits correctly.
  const vec3 p_i = vec3(  //
	  intBitsToFloat(floatBitsToInt(worldPosition.x) + ((worldPosition.x < 0) ? -of_i.x : of_i.x)),
	  intBitsToFloat(floatBitsToInt(worldPosition.y) + ((worldPosition.y < 0) ? -of_i.y : of_i.y)),
	  intBitsToFloat(floatBitsToInt(worldPosition.z) + ((worldPosition.z < 0) ? -of_i.z : of_i.z)));
  // Use a floating-point offset instead for points near (0,0,0), the origin.
  const float origin     = 1.0f / 32.0f;
  const float floatScale = 1.0f / 65536.0f;
  return vec3(  //
	  abs(worldPosition.x) < origin ? worldPosition.x + floatScale * normal.x : p_i.x,
	  abs(worldPosition.y) < origin ? worldPosition.y + floatScale * normal.y : p_i.y,
	  abs(worldPosition.z) < origin ? worldPosition.z + floatScale * normal.z : p_i.z);
}

#endif
