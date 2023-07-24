#include "stdodt.h"

#include "Image.h"
//#include "ResourceStorage.h"

//const char* ImageType::Id = "ImageType";
//ImageType::ImageType()
//	:ResourceType( 1, Id )
//{
//}
//ImageType::~ImageType(){
//}
//Resource* ImageType::newInstance(){
//	return new Image();
//}

Image::Image( const String& name )
    :Resource( name )
{
}
Image::~Image(){
	if( _pixels ){
		delete[] _pixels;
		_pixels = null;
	}
	_width = 0;
	_height = 0;
}
float c2f( unsigned char v ){
	// -128 to 127
	float f = v;
	//f += 128;
	f /= 255;
	ASSERT( 0 <= f && f <= 1 );
	return f;
}
unsigned char f2c( float v ){
	if( v <= 0 )
		return 0;
	if( 1 <= v )
		return 255;
	return ( v * 255 );
}
glm::vec4 Image::pixel( uint x, uint y ) const {
	ASSERT( x < _width ); ASSERT( y < _height );
	uint xy = x + ( y * _width );
	uint i = xy * 4;
	unsigned char* rgba = &_pixels[ i ];
    //logDebug( "i", x, y, "i", i, "rgba", (int) _pixels[ i + 0 ], (int) _pixels[ i + 1 ], (int) _pixels[ i + 2 ], (int) _pixels[ i + 3 ]  );
    return glm::vec4( c2f( rgba[0] ), c2f( rgba[1] ), c2f( rgba[2] ), c2f( rgba[3] ) );
}
void Image::set( uint x, uint y, const glm::vec4& color ){
	ASSERT( x < _width ); ASSERT( y < _height );
	uint xy = x + ( y * _width );
	uint i = xy * 4;
	unsigned char* rgba = &_pixels[ i ];
	*rgba = f2c( color.r ); rgba++;
	*rgba = f2c( color.g ); rgba++;
	*rgba = f2c( color.b ); rgba++;
	*rgba = f2c( color.a ); rgba++;
}
void Image::fill( const glm::vec4& color ){
	unsigned char r = f2c( color.r );
	unsigned char g = f2c( color.g );
	unsigned char b = f2c( color.b );
	unsigned char a = f2c( color.a );
	unsigned char* rgba = _pixels;
	for( uint x = 0; x < _width; x++ ){
		  for( uint y = 0; y < _height; y++ ){
			  *rgba = r; rgba++;
			  *rgba = g; rgba++;
			  *rgba = b; rgba++;
			  *rgba = a; rgba++;
		}
	}
}
void Image::create( uint width, uint height ){
	assert( 0 < width ); assert( 0 < height );
	assert( _width == 0 && _height == 0 && _pixels == null );
	_width = width;
	_height = height;
	uint size = _width * _height * 4;
	_pixels = new unsigned char[ size ];
}
//void Image::load( BinaryFileReader& reader ){
//	Resource::load( reader );
//	assert( _width == 0 && _height == 0 && _pixels == null );
//	uint8_t magicnumber = reader.read_uint8();
//	assert( magicnumber == 'i' );
//	_width = reader.read_uint32();
//	_height = reader.read_uint32();
//	uint32_t texSize = reader.read_uint32();
//	assert( texSize == _width * _height * 4 );
//	_pixels = new unsigned char[ texSize ];
//	reader.read_block( _pixels, texSize );
//	magicnumber = reader.read_uint8();
//	assert( magicnumber == 'I' );
//}
//void Image::save( BinaryFileWriter& writer ) const {
//	Resource::save( writer );
//	assert( 0 < _width && 0 < _height && _pixels != null );
//	writer.write_uint8( 'i' );
//	writer.write_uint32( _width );
//	writer.write_uint32( _height );
//	uint32_t texSize = _width * _height * 4;
//	writer.write_uint32( texSize );
//	writer.write_block( _pixels, texSize );
//	writer.write_uint8( 'I' );
//}
//#ifdef ODTDEBUG
//#include "external/stb/stb_image.h"
//void Image::import( const String& filename ){
//	assert( _width == 0 && _height == 0 && _pixels == null );
//	int texWidth, texHeight, texChannels;
//	stbi_uc* pixels = stbi_load( filename.asCStr(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
//	if( !pixels ){
//		logError( "import image failed", filename );
//		assert( false );
//	}
//	_pixels = pixels;
//	_width = texWidth;
//	_height = texHeight;
//	logDebug( "Image::import\t   b: ", ( _width * _height * 4 ), "\tkb: ", ( _width * _height * 4 / 1024 ), "\tmb: ", ( _width * _height * 4 / 1024  / 1024  ), "\tfn: ", filename );
//}
//float i16toc( stbi_us i16 ){
//	// stbi_us = unsigned short = 0 to 65535
//	float f = i16;
//	f /= 65535;
//	ASSERT( 0 <= f && f <= 1 );
//	return f;
//}
//Array2D<vec4>* Image::Import16( const String& filename ){
//	int width = -1;
//	int height = -1;
//	int channels = -1;
//	stbi_us* pixels = stbi_load_16( filename.asCStr(), &width, &height, &channels, STBI_rgb_alpha );
//	assert( 0 < width ); assert( 0 < height ); // assert( channels == STBI_rgb_alpha );
//	Array2D<vec4>* data = new Array2D<vec4>( width, height );
//	stbi_us* rgba = pixels;
//	for( uint x = 0; x < width; x++ ){
//		for( uint y = 0; y < height; y++ ){
//			  float r = i16toc( *rgba ); rgba++;
//			  float g = i16toc( *rgba ); rgba++;
//			  float b = i16toc( *rgba ); rgba++;
//			  float a = i16toc( *rgba ); rgba++;
//			  vec4& pixel = data->get( x, y );
//			  pixel[0] = r;
//			  pixel[1] = g;
//			  pixel[2] = b;
//			  pixel[3] = a;
//		}
//	}
//	stbi_image_free( pixels );
//	return data;
//}
//#endif

//void Image::fixAlphaErrorToBlack(){
//	for( uint x = 0; x < width(); x++ ){
//		for( uint y = 0; y < height(); y++ ){
//			vec4 c = pixel( x, y );
//			if( c.a < 0.004 ){
//				set( x, y, vec4( 0, 0, 0, 0 ) );
//			}
//		}
//	}
//}
//void Image::convertGrayToAlpha(){
//	for( uint x = 0; x < width(); x++ ){
//		for( uint y = 0; y < height(); y++ ){
//			vec4 c = pixel( x, y );
//			float g = ( c.x + c.y + c.z ) / 3;
//			set( x, y, vec4( 1, 1, 1, g ) );
//		}
//	}
//}

