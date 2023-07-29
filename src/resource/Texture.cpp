#include "stdodt.h"

#include "Texture.h"
#include "ResourceType.h"
#include "ResourceStorage.h"


//Texture::Texture( const String& filename )
//	:_filename( filename )
//{
//	int todo_rename_to_image;
//}
//Texture::Texture( Image* data )
//	:_data( data )
//{
//}
//Texture::~Texture(){
//}
//void Texture::load( Image& data ){
//	assert( data._pixels == null );
//	BinaryFileReader reader( _filename );
//	data._width = reader.read_uint32();
//	data._height = reader.read_uint32();
//	uint pixel_size = reader.read_uint32();
//	assert( data._width * data._height * 4 == pixel_size );
//	data._pixels = new unsigned char[ pixel_size ];
//	reader.read_block( data._pixels, pixel_size );

//}
////void Texture::setTileCount( int x, int y ){
////	_tileCountX = x;
////	_tileCountY = y;
////}

Texture::Texture(){
}
Texture::~Texture(){
	_image = null;
}
ResourceType* Texture::type() const{
	return Singleton::Get<TextureType>();
}
void Texture::setImage( Image* image ){
	assert( _image == null );
	assert( image );
	_image = image;
}
void Texture::load( BinaryFileReader& reader ){
//	Resource::load( reader );
	assert( _image == null );
	ResourceReader* rr = asserted( dynamic_cast<ResourceReader*>( &reader ) );
	_image = asserted( dynamic_cast<Image*>( rr->read_reference() ) );
}
void Texture::save( BinaryFileWriter& writer ) const {
//	Resource::save( writer );
	assert( _image );
	ResourceWriter* rw = asserted( dynamic_cast<ResourceWriter*>( &writer ) );
	rw->write_reference( _image );
}
