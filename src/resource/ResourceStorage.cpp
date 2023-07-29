#include "stdodt.h"
#include "ResourceStorage.h"
#include "Resource.h"
#include "ResourceType.h"

ResourceReader::ResourceReader( const String& filename )
	:BinaryFileReader( filename )
{
}
ResourceReader::~ResourceReader(){
	_resources.removeAll();
}
Resource* ResourceReader::read_resource(){
	ResourceReader& reader = *this;
	uint8_t typeId = reader.read_uint8();
	if( _types == null )
		_types = ResourceTypes::Get();
	ResourceType* type = _types->get( typeId );
	Resource* resource = type->newInstance();
	_resources.add( resource );
	resource->load( reader );
	return resource;
}
Resource* ResourceReader::read_reference(){
	uint8_t m = read_uint8();
	assert( m == 'r' );
	uint32_t i = read_uint32();
	assert( i < _resources.size() );
	return _resources.get( i );
}

ResourceWriter::ResourceWriter( const String& filename )
	:BinaryFileWriter( filename )
{
}
ResourceWriter::~ResourceWriter(){
	_resources.removeAll();
}
void ResourceWriter::write( Resource* resource ){
	assert( resource );
	_resources.add( resource );
	write_uint8( resource->type()->id() );
	resource->save( *this );
}
void ResourceWriter::write_reference( Resource* resource ){
	int i = _resources.indexOf( resource );
	assert( 0 <= i );
	write_uint8( 'r' );
	write_uint32( i );
}
