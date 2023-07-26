#pragma once

#include "Debugging.h"

class String
{
public:
	static String Repeated( const String& txt, int count );
private:
	std::string _value;
public:
	String();
	String( const char* value );
	String( const std::string& value );
	String( const String& value );
	~String();
public:
	int length() const{ return _value.length(); }
	bool isEmpty() const{ return _value.empty(); }
public:
	char get( int i ) const;
public:
	bool equals( const String& value ) const;
	bool equalsIgnoreCase( const String& value ) const;
	bool operator==( const String& value ) const;
	bool operator!=( const String& value ) const;
	bool startsWith( const String& prefix ) const;
	bool endsWith( const String& postfix ) const;
	bool contains( const String& part ) const;
	static int Compare( const String& a, const String& b );
public:
	String mid( int start ) const;
	String mid( int start, int count ) const;
public:
	String toLower() const;
public:
	void clear();
	void operator=( const String& value );
public:
	String operator+( const String& value ) const;
	void operator+=( const String& value );
public:
	void insert( int index, char c );
	void remove( int index );
	void replaceAll( char s, char t );
	void replaceAll( const String& s, const String& t );
public:
	std::list<String> split( char delimiter ) const;
public:
	float toFloat() const;
	int toInt() const;
public:
	static String FromInt( int value );
	static String FromFloat( float value );
public:
	const std::string& asStd() const{ return _value; }
	const char* asCStr() const{ return _value.c_str(); }
public:
//	struct HashFunction {
//		size_t operator()( const String& string ) const {
//			return hash<std::string>{}( _value );
//		}
//	};
};

std::ostream& operator<<( std::ostream &os, const String& value );
