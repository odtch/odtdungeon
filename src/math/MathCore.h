#pragma once

#include "utils/Debugging.h"
#include "utils/Logger.h"

struct Vec3;
//struct Vec3Path;
struct Orientation;
struct PosOri;
struct Ray;
struct Plane;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

//class BinaryFileReader;
//class BinaryFileWriter;

inline bool fequals( float a, float b, float maxDiff = 0.001f ){
    return std::fabs( a - b ) <= maxDiff;
}
//bool isSame( const vec3& a, const vec3& b, float maxDiff );

inline float gradToPi( float grad ){ return ( grad * M_PI ) / 180.0f; }
inline float piToGrad( float pi ){ return ( pi * 180.0f ) / M_PI; }

float lerpf( float a, float b, float f );

class Projection {
public:
	enum Type {
		Perspective,
		Orthographic
	};
};
