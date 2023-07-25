#include "stdodt.h"

#include "Resource.h"
//#include "ResourceStorage.h"

Resource::Resource( const String& name )
    :_name( name )
{
}
//Resource::Resource( const String& id )
//	:_id( id )
//{
////	static uint nextDebugNo = 0;
////	_debugno = ( nextDebugNo ++ );
////	logDebug( _debugno );
//}
Resource::~Resource(){
}
void Resource::load( BinaryFileReader& reader ){
	assert( false );
}
void Resource::save( BinaryFileWriter& writer ) const {
	assert( false );
}
//void Resource::setId( const String& id ){
//	//ASSERT( _id.isEmpty() || _id.startsWith( "mesh " ) );
//	ASSERT( !id.isEmpty() );
//	_id = id;
//}
//void Resource::load( BinaryFileReader& reader ){
//	if( _id.isEmpty() ){
//	} else {
//		ASSERT( _id.startsWith( "mesh " ) );
//		_id = String();
//	}
//	reader.read_string( _id );
//}
//void Resource::save( BinaryFileWriter& writer ) const {
//	writer.write_string( _id );
//}

