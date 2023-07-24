#include "stdodt.h"
#include "Vec4.h"

const Vec4 Vec4::Null( 0, 0, 0, 0 );
const Vec4 Vec4::One( 1, 1, 1, 1 );

std::ostream &operator<<( std::ostream &os, const Vec4& v ){
	return os << v.toString();
}

Vec4 Vec4::Lerp( const Vec4& a, const Vec4& b, float f ){
	if( f < 0 ){
        std::cout << "Vec4::Lerp < 0\n";
		f = 0;
	} else if( 1 < f ){
        std::cout << "Vec4::Lerp > 1\n";
		f = 1;
	}
	return a * ( 1.0f - f ) + b * f;
}
