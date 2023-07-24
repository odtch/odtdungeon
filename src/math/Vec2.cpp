#include "stdodt.h"
#include "Vec2.h"

const Vec2 Vec2::Null( 0, 0 );

std::ostream &operator<<( std::ostream &os, const Vec2& v ){
	return os << v.toString();
}
Vec2 Vec2::Lerp( const Vec2& a, const Vec2& b, float f ){
	if( f < 0 ){
		std::cout << "Vec2::Lerp < 0\n";
		f = 0;
	} else if( 1 < f ){
		std::cout << "Vec2::Lerp > 1\n";
		f = 1;
	}
	return a * ( 1.0f - f ) + b * f;
}
