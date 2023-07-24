#include "stdodt.h"
#include "Vec3.h"

const Vec3 Vec3::Null( 0, 0, 0 );
const Vec3 Vec3::Forward( 0, 1, 0 );
const Vec3 Vec3::Backward( 0, -1, 0 );
const Vec3 Vec3::Right( 1, 0, 0 );
const Vec3 Vec3::Left( -1, 0, 0 );
const Vec3 Vec3::Up( 0, 0, 1 );
const Vec3 Vec3::Down( 0, 0, -1 );
const Vec3 Vec3::OneOneOne( 1, 1, 1 );

std::ostream &operator<<( std::ostream &os, const Vec3& v ){
	return os << v.toString();
}

Vec3 Vec3::Lerp( const Vec3& a, const Vec3& b, float f ){
	if( f < 0 ){
		std::cout << "Vec3::Lerp < 0\n";
		f = 0;
	} else if( 1 < f ){
		std::cout << "Vec3::Lerp > 1\n";
		f = 1;
	}
	return a * ( 1.0f - f ) + b * f;
}
