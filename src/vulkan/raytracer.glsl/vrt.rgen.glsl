#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_debug_printf : require
#include "vrt.common.glsl.h"
#include "vrt.globals.glsl.h"
//#include "vulkan/raytracer/VulkanRaytracerData.h"

// Binding BINDING_IMAGEDATA in set 0 is a storage image with four 32-bit floating-point channels,
// defined using a uniform image2D variable.
layout( binding = 0, set = 0, rgba32f ) uniform image2D storageImage;
//layout(binding = BINDING_IMAGEDATA, set = 0, rgba32f) uniform image2D storageImage;
//layout(binding = BINDING_TLAS, set = 0) uniform accelerationStructureEXT tlas;
layout( binding = 3, set = 0 ) uniform accelerationStructureEXT tlases[];


layout(location = 0) rayPayloadEXT ORayPayload pld;

#include "vrt.oray.glsl.h"

vec4 calcPixelColor( float pixelx, float pixely ){
	/*
	color-table
	*/
	if( pixelx < 256 + 40 && pixely < 100 ){
		float px = pixelx;
		px -= 20;
		float py = pixely;
		py -= 10;
		vec4 bc = vec4( 0.5, 0.5, 0.5, 0.0 );
		if( px < 0 )return bc;
		if( py < 0 ){
			if( 127 < px && px < 128 )
				return vec4( 1, 1, 1, 0 );
			return bc;
		}
		if( px < 256 ){
			float c = px / 255;
			if( py < 20 )return vec4( c, 0, 0, 1 );
			if( py < 40 )return vec4( 0, c, 0, 1 );
			if( py < 60 )return vec4( 0, 0,c, 1 );
			if( py < 80 )return vec4( c, c, c, 1 );
		}
		if( px < 256 + 20 )
			return bc;

	}
	/* */
//	if( pixelx < 256 && pixely < 256 ){
//		int texture_index = 9;
//		vec2 texture_coord = vec2( pixelx / 256, pixely / 256 );
//		vec4 texture_color = texture( textures[ texture_index ], texture_coord );
//		return texture_color; //vec4( 1, 1, 1, 1 );
//	}
	vec3 camera_position = vec3( pixelx, pixely, 1000 );
	vec3 camera_direction = vec3( 0, 0, -1 );
	uint layer_index = 0;
	vec4 color = ORayResolve( vec2( pixelx, pixely ), layer_index, camera_position, camera_direction );
	return color;
	/* */
}
void main(){
	const ivec2 resolution = imageSize(storageImage);
	const ivec2 pixel = ivec2(gl_LaunchIDEXT.xy);
	if( globals.last != 1094 ){
		if( pixel.x == 0 && pixel.y == 0 ){
			debugPrintfEXT( "globals.last invalid %d %d %d\n", globals.reserve92, globals.reserve93, globals.last );
			for( int v = 0; v < 3; v++ ){
				debugPrintfEXT( " .l %d: %d %d %d\n", v, globals.layers[v].reserve92, globals.layers[v].reserve93, globals.layers[v].last );
			}
		}
	}
	if( ( pixel.x >= resolution.x ) || ( pixel.y >= resolution.y ) ){
		return; // If the pixel is outside of the image, don't do anything:
	}
	vec4 pixelcolor = vec4( 0 );
	const int aa_count = 1;
	for( int aa = 0; aa < aa_count; aa++ ){
		float px = pixel.x;
		float py = pixel.y;
		if( aa == 0 ){
			px += 0.7f; py += 0.3f;
		} else if( aa == 1 ){
			px += 0.33f; py += 0.67f;
		} else {
			px += 0.5f; py += 0.5f;
		}
//		pixelcolor.x = px / resolution.x;
//		pixelcolor.y = py / resolution.y;
		pixelcolor += calcPixelColor( px, py );
	}
	pixelcolor /= aa_count;
	imageStore( storageImage, pixel, pixelcolor );
	/*

	// const uint sample_batch = globals.sample_batch;
	// State of the random number generator with an initial seed.
	// pld.rngState = uint(( sample_batch * resolution.y + pixel.y ) * resolution.x + pixel.x);
	// The sum of the colors of all of the samples.
	vec3 summedPixelColor = vec3(0.0);
	// Limit the kernel to trace at most 64 samples.
	//  const int NUM_SAMPLES = 64;
	//  for(int sampleIdx = 0; sampleIdx < NUM_SAMPLES; sampleIdx++){
	// Rays always originate at the camera for now. In the future, they'll
	// bounce around the scene.
	// vec3 rayOrigin = camera_position;
	// Compute the direction of the ray for this pixel. To do this, we first
	// transform the screen coordinates to look like this, where a is the
	// aspect ratio (width/height) of the screen:
	//           1
	//    .------+------.
	//    |      |      |
	// -a + ---- 0 ---- + a
	//    |      |      |
	//    '------+------'
	//          -1
	// Use a Gaussian with standard deviation 0.375 centered at the center of
	// the pixel:
	// const vec2 randomPixelCenter = vec2(pixel) + vec2(0.5) + 0.375 * randomGaussian(pld.rngState);
	// const vec2 screenUV          = vec2((2.0 * randomPixelCenter.x - resolution.x) / resolution.y,    //
	//                           -(2.0 * randomPixelCenter.y - resolution.y) / resolution.y);  // Flip the y axis
	//const uvec2 pixel = gl_GlobalInvocationID.xy;
	//vec2 ali_pixel = vec2( pixel.x + aliasing_offsets[a].x, pixel.y + aliasing_offsets[a].y );
	vec2 ali_pixel = vec2( pixel.x, pixel.y  );
	vec3 pixel_color = vec3( 0 );
	for( uint v = 0; v < globals.viewport_count; v++ ){
		if( globals.layers[v].viewport_x0 <= pixel.x && pixel.x <= globals.layers[v].viewport_x1
				&& globals.layers[v].viewport_y0 <= pixel.y && pixel.y <= globals.layers[v].viewport_y1 ){
			vec2 pixel_factor_in_viewport = vec2(
									( ali_pixel.x - globals.layers[v].viewport_x0 + 0.5f ) / globals.layers[v].viewport_width,
									( ali_pixel.y - globals.layers[v].viewport_y0 + 0.5f ) / globals.layers[v].viewport_height );
			vec4 pixel_in_front_plane =
									( globals.layers[v].camera_near_plane_lt * ( 1.0 - pixel_factor_in_viewport.x )
									+ globals.layers[v].camera_near_plane_rt * ( pixel_factor_in_viewport.x )
									) * ( 1.0 - pixel_factor_in_viewport.y )
									+
									( globals.layers[v].camera_near_plane_lb * ( 1.0 - pixel_factor_in_viewport.x )
									+ globals.layers[v].camera_near_plane_rb * ( pixel_factor_in_viewport.x )
									) * ( pixel_factor_in_viewport.y );
			const vec3 camera_position = globals.layers[ v ].camera_position.xyz;
			// Define the field of view by the vertical slope of the topmost rays:
			vec3 camera_direction = normalize( pixel_in_front_plane.xyz - globals.layers[v].camera_position.xyz );
			//vec3 accumulatedRayColor = vec3(1.0);  // The amount of light that made it to the end of the current ray.
			// Limit the kernel to trace at most 32 segments.
			//for(int tracedSegments = 0; tracedSegments < 32; tracedSegments++){
			pld.depth = 0;
			pld.color = vec3( 0 );
			traceRayEXT( tlases[ globals.layers[v].tlas_index ],
					  gl_RayFlagsOpaqueEXT,  // Ray flags, here saying "treat all geometry as opaque"
					  0xFF,                  // 8-bit instance mask, here saying "trace against all instances"
					  0,                     // SBT record offset
					  0,                     // SBT record stride for offset
					  0,                     // Miss index
					  camera_position,
					  0.0,                   // Minimum t-value
					  camera_direction,
					  100000.0,               // Maximum t-value
					  0);                    // Location of payload
	//	  accumulatedRayColor *= pld.color;
	//      pld.rayHitSky = true;
	//      accumulatedRayColor = pld.color;
	//	  summedPixelColor += pld.color;
			  pixel_color += pld.color;
		}
	}
	* /
	//imageStore( storageImage, pixel, vec4( pixel_color, 0.0));
//      if(pld.rayHitSky)
//      {
//        // Done tracing this ray.
//        // Sum this with the pixel's other samples.
//        // (Note that we treat a ray that didn't find a light source as if it had
//        // an accumulated color of (0, 0, 0)).
//        summedPixelColor += accumulatedRayColor;

//        //break;
//      }
//      else
//      {
//        // Start a new segment
//        rayOrigin    = pld.rayOrigin;
//        rayDirection = pld.rayDirection;
//      }
//    }

/ *
  // Blend with the averaged image in the buffer:
  vec3 averagePixelColor = summedPixelColor / float(NUM_SAMPLES);
  if( sample_batch != 0 ){
	// Read the storage image:
	const vec3 previousAverageColor = imageLoad(storageImage, pixel).rgb;
	// Compute the new average:
	averagePixelColor =
		( sample_batch * previousAverageColor + averagePixelColor) / ( sample_batch + 1 );
  }
  //vec3 averagePixelColor = summedPixelColor;
  // Set the color of the pixel `pixel` in the storage image to `averagePixelColor`:
  //imageStore(storageImage, pixel, vec4(averagePixelColor, 0.0));
  */
}
