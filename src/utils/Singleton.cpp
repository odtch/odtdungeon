#include "stdodt.h"

#include "Singleton.h"
#include "Logger.h"

const char* IdGenerator::Create( const String& a, const String& b ){
	String string( a + "." + b );
	std::string* str = new std::string( string.asStd() );
	return str->c_str();
}
const char* IdGenerator::Create( const String& a, const String& b, const String& c ){
	String string( a + "." + b + "." + c );
	std::string* str = new std::string( string.asStd() );
	return str->c_str();
}


List<Singleton*> Singleton::_singletons;

Singleton* Singleton::GetById( const char* id ){
	Singleton* singleton = FindById( id );
	if( singleton )
		return singleton;
	logError( "Singleton not found", id );
	assert( false );
}
Singleton* Singleton::FindById( const char* id ){
	assert( id );
	for( Singleton* singleton : _singletons ){
		if( singleton->id() == id )
			return singleton;
	}
	return null;
}
void Singleton::Shutdown(){
	while( !_singletons.isEmpty() ){
		Singleton* singleton = _singletons.last();//.takeLast();
		delete singleton;
	}
}

Singleton::Singleton( const char* id )
	:_id( id )
{
	assert( _id );
#ifdef ODTDEBUG
	//logDebug( "Singleton", _id );
	if( FindById( _id ) != null ){
		logError( "Singleton id not unique", _id );
	}
#endif
	_singletons.add( this );
}
Singleton::~Singleton(){
	_singletons.remove( this );
	_id = null;
}

