#include "stdodt.h"

#include "String.h"
//#include <locale>

String String::Repeated( const String& txt, int count ){
	std::string result;
	assert( 0 <= count && count < 200 );
	for( int i = 0; i < count; i++ )
		result += txt.asStd();
	return result;
}
String::String(){
}
String::String( const char* value ){
	assert( value );
	_value = std::string( value );
}
String::String(const std::string& value)
	:_value( value )
{
}
String::String(const String& value)
	:_value( value._value )
{
}
String::~String(){
}
char String::get(int i) const {
	assert( 0 <= i && i < length() );
	return _value[ i ];
}
bool String::equals(const String& value) const {
	return _value.compare( value._value ) == 0;
}
bool String::equalsIgnoreCase( const String& value ) const {
	unsigned int sz = _value.size();
	if (value._value.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(_value[i]) != tolower(value._value[i]))
			return false;
	return true;
}
bool String::operator==(const String& value) const {
	return equals( value );
}
bool String::operator!=(const String& value) const {
	return !equals( value );
}
bool String::startsWith( const String& prefix ) const {
	if( this->length() < prefix.length() )
		return false;
	for( int i = 0; i < prefix.length(); i++ ){
		char m = _value[i];
		char p = prefix._value[i];
		if( m != p )
			return false;
	}
	return true;
//	if( _value.rfind( prefix._value ) == 0 ){
//		return true;
//	} else {
//		return false;
//	}
}
bool String::endsWith(const String& postfix) const {
	if( length() < postfix.length() )
		return false;
	return 0 == this->asStd().compare( length() - postfix.length(), postfix.asStd().length(), postfix.asStd() );
}
bool String::contains(const String& part) const{
	if( _value.rfind( part._value ) != std::string::npos ){
		return true;
	} else {
		return false;
	}
}
int String::Compare( const String& a, const String& b ){
//	int rc = a.asStd().compare( b.asStd() );
//	if( rc < 0 ){
//		//std::cout << "a";
//	} else if( rc == 0 ){
//		if( a.isEmpty() ){
//			//std::cout << "b";
//		} else {
//			//std::cout << "2b";
//		}
//	} else {
//		//std::cout << "c";
//	}
	return a.asStd().compare( b.asStd() );
}
String String::mid( int start ) const {
	return String( _value.substr( start ) );
}
String String::mid( int start, int count ) const {
	return String( _value.substr( start, count ) );
}
String String::toLower() const {
	String result( *this );
	for( char& c : result._value ){
		c = tolower( c );
	}
	return result;
}
void String::clear(){
	_value.clear();
}
void String::operator=( const String& value ){
	_value = value._value;
}

String String::operator+(const String& value) const
{
	return String( _value + value._value );
}
void String::operator+=( const String& value ){
	_value += value._value;
}
void String::insert( int index, char c ){
	assert( 0 <= index && index <= length() );
	assert( 1 < c );
	_value.insert( index, 1, c );
}
void String::remove(int index){
	assert( 0 <= index && index < length() );
	_value.erase( index, 1 );
}
void String::replaceAll( char s, char t ){
	std::replace( _value.begin(), _value.end(), s, t );
}
void String::replaceAll( const String& s, const String& t ){
	assert( !s.isEmpty() );
	while( true ){
		size_t start_pos = _value.find( s._value );
		if( start_pos == std::string::npos )
			return;
		_value.replace( start_pos, s.length(), t._value );
	}
}
std::list<String> String::split( char delimiter ) const {
	std::list<String> result;
	size_t last = 0; size_t next = 0;
	while( ( next = _value.find( delimiter, last ) ) != std::string::npos ){
		std::string part = _value.substr( last, next-last );
		result.push_back( String ( part ) );
		last = next + 1;
	}
	std::string rest = _value.substr( last );
	result.push_back( String( rest ) );
	return result;
}
float String::toFloat() const{
	try{
		return std::stof( asStd() );
//	} catch( std::invalid_argument ex ){
//		std::cout << ""
//		throw std::runtime_error( "error converting string to float" );
//	} catch( std::out_of_range ex ){
//		throw std::runtime_error( "error converting string to float" );
	} catch( ... ){
		std::cout << "String::toFloat failed " << asStd() << "\n";
		throw std::runtime_error( "error converting string to float" );
	}
}
int String::toInt() const {
	try{
		return std::stoi( asStd() );
//	} catch( std::invalid_argument ex ){
//		std::cout << ""
//		throw std::runtime_error( "error converting string to float" );
//	} catch( std::out_of_range ex ){
//		throw std::runtime_error( "error converting string to float" );
	} catch( ... ){
		std::cout << "String::toInt failed " << asStd() << "\n";
		throw std::runtime_error( "error converting string to int" );
	}
}
String String::FromInt( int value ){
	return String( std::to_string( value ) );
}
String String::FromFloat( float value ){
	return String( std::to_string( value ) );
}
std::ostream& operator<<( std::ostream &os, const String& value ){
	return os << value.asStd();
}
