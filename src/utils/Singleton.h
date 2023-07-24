#pragma once

#include "List.h"
#include "Debugging.h"
#include "String.h"

class IdGenerator {
public:
	static const char* Create( const String& a, const String& b );
	static const char* Create( const String& a, const String& b, const String& c );
};

class Singleton
{
	DEBUGCOUNTER( Singleton )
	ODTNOCOPY( Singleton )
private:
	static List<Singleton*> _singletons;	
public:
	template<typename T>
	static T* Get(){
		return (T*)GetById( T::Id );
	}
	static Singleton* GetById( const char* id );
	static Singleton* FindById( const char* id );
	static void Shutdown();
private:
	const char* _id;
public:
	explicit Singleton( const char* id );
	virtual ~Singleton();
public:
	const char* id() const { return _id; }
};
