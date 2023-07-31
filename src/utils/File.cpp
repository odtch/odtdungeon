#include "stdodt.h"

#include "File.h"
#include "Logger.h"
//#include "math/OdtMath.h"

#include <iostream>
#include <filesystem>

#define assertStreamValid( s, msg )\
	if( s.fail() ){ \
		std::cerr << "stream failed " << msg << ": " << errno << ": " << strerror( errno ) << " at " << __LINE__ << "\n"; \
		othrow( msg ); \
	}

bool File::Exists( const String& filename ){
	return std::filesystem::exists( filename.asStd() );
}
void File::DeleteFile( const String& filename ){
	if( !std::filesystem::remove( filename.asStd() ) )
		othrow( "cant delete file" );
}
void File::DeleteFileIfExists( const String& filename ){
	if( Exists( filename ))
		DeleteFile( filename );
}
void File::Move( const String& src, const String& trg ){
	std::filesystem::rename( src.asStd(), trg.asStd() );
}
void File::CreateDirectory( const String& path ){
	if( !std::filesystem::create_directory( std::filesystem::path( path.asStd() ) ) ){
		logError( "File::CreateDirectory failed", path );
		othrow( "cant create directory " );
	}
}
void File::CreateDirectoryIfNotExists( const String& path ){
	if( !Exists( path ) )
		CreateDirectory( path );
}
String File::LoadText( const String& filename ){
	std::ifstream t( filename.asStd() );
	if( t.fail() ){ logError( filename ); }
	assertStreamValid( t, "cant open text file for read" );
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);
	return String( buffer );
}
void File::CreateText( const String& filename, const String& content ){
	if( Exists( filename ) ){
		logError( "file already exists", filename );
		othrow( "File already exists" );
	}
	std::ofstream _stream;
	_stream.open( filename.asStd(), std::ios::out | std::ios::trunc | std::ios::binary );
	assertStreamValid( _stream, filename.asStd() + ": cant open text file for create" );
	_stream.write( content.asCStr(), content.length() );
	_stream.close();
}
String File::GetNameFromPath( const String& filename ){
	std::filesystem::path path( filename.asStd() );
	String name( path.stem().u8string() );
	return name;
}
void FileFind( const String& directory, const String& nameprefix, const File::FindOptions& options, std::function<void( const File::Entry& entry )> handler ){
	File::Entry entry;
	for( const std::filesystem::directory_entry& fsentry : std::filesystem::directory_iterator( directory.asStd() ) ){
		const auto& path = fsentry.path();
		if( nameprefix.isEmpty() ){
			entry.name = path.stem().u8string();
		} else {
			entry.name = nameprefix + "/" + path.stem().u8string();
		}
		if( fsentry.is_regular_file() ){
			if( !options.filterDotExt.isEmpty() ){
				const auto& ext = path.extension().u8string();
				if( options.filterDotExt.asStd() == ext ){
				} else {
					continue;
				}
			}
			entry.path = path.u8string();
			handler( entry );
		}
		if( fsentry.is_directory() ){
			if( options.recursive ){
				FileFind( fsentry.path().u8string(), entry.name, options, handler );
			}
		}
	}
}
void File::Find( const String& directory, const FindOptions& options, std::function<void( const Entry& entry )> handler ){
	FileFind( directory, String(), options, handler );
}
void File::GetFiles( const String& directory, const FindOptions& options, List<Entry*>& files ){
	Find( directory, options, [&files]( const Entry& entry ){
		files.add( new Entry( entry ) );
	} );
}
void File::SortFiles( List<Entry*>& files ){
	files.sort( []( Entry* a, Entry* b ){
		return String::Compare( a->name, b->name );
	});
}

File::File(){
}
File::~File(){
}
void File::create( const String& path, std::function<void (BinaryFileWriter&)> saver ){
	assert( isOpen() == false );
	assert( path.isEmpty() == false );
	if( File::Exists( path ) ){
		logError( "File already exists:", path );
		othrow( "File already exists" );
	}
	BinaryFileWriter writer( path );
	saver( writer );
	writer.close();
	_path = path;
	logDebug( "File::created", _path );
}
void File::load( const String& path, std::function<void (BinaryFileReader&)> loader ){
	assert( isOpen() == false );
	{
		BinaryFileReader reader( path );
		loader( reader );
	}
	_path = path;
	// logDebug( "File::loaded", _path );
}
void File::save( std::function<void (BinaryFileWriter&)> saver ){
	assert( isOpen() == true );
	String path = _path;
	_path = String();
	BinaryFileWriter writer( path, BinaryFileWriter::Overwrite );
	saver( writer );
	writer.close();
	_path = path;
	logDebug( "File::saved", _path );
}
bool File::isOpen() const{
	return _path.isEmpty() == false;
}
void File::close(){
	_path = String();
}

BinaryFileReader::BinaryFileReader( const String& filename ){
	//logDebug( "open file", filename );
	_stream.open( filename.asStd(), std::ios::in | std::ios::binary );
	assertStreamValid( _stream, std::string( "cant open binary file for read: " ) + filename.asStd() );
}
BinaryFileReader::~BinaryFileReader(){
	char c;
	if( _stream >> c ){
		logError( "BinaryFileReader ~ not eof" );
#ifdef ODTDEBUG
		int logcount = 0;
		do{
			std::cout << (int)c << " ";
			if( _stream >> c ){
			} else {
				break;
			}
			logcount++;
		} while( !_stream.eof() && logcount < 100 );
		std::cout << "\n";
#endif
		_stream.close();
		othrow( "File not fully loaded" );
	}
	assert( _stream.eof() );
	_stream.close();
}
uint8_t BinaryFileReader::read_uint8(){
	uint8_t value = 0;
	_stream.read( (char*) &value, sizeof( value ) );
	return value;
}
uint32_t BinaryFileReader::read_uint32(){
	uint32_t value = 0;
	_stream.read( (char*) &value, sizeof( value ) );
	return value;
}
void BinaryFileReader::read_magicnumber( uint32_t expected ){
	uint32_t magicnumber = read_uint32();
	assert( expected == magicnumber );
}
float BinaryFileReader::read_float(){
	float value = 0;
	_stream.read( (char*) &value, sizeof( value ) );
	return value;
}
bool BinaryFileReader::read_bool(){
	uint8_t value = read_uint8();
	if( value == 0 )
		return false;
	if( value == 1 );
		return true;
	logError( "invalid value", value );
	assert( false );
}
void BinaryFileReader::read_string( String& value ){
	ASSERT( value.isEmpty() );
	uint32_t l = read_uint32();
	assert( l < 254 );
	_stream.read( _buffer, l );
	auto s = std::string( _buffer, l );
	value = String( s );
}
glm::vec3 BinaryFileReader::read_vec3(){
	float x = read_float();
	float y = read_float();
	float z = read_float();
	return glm::vec3( x, y, z );
}
glm::vec4 BinaryFileReader::read_vec4(){
	float x = read_float();
	float y = read_float();
	float z = read_float();
	float w = read_float();
	return glm::vec4( x, y, z, w );
}
//void BinaryFileReader::read_vec3( Vec3& value ){
//	value.setX( read_float() );
//	value.setY( read_float() );
//	value.setZ( read_float() );
//}
//void BinaryFileReader::read_quat(glm::quat& value){
//	value.x = read_float();
//	value.y = read_float();
//	value.z = read_float();
//	value.w = read_float();
//}
void BinaryFileReader::read_block(void* data, size_t size){
	assert( data );
	assert( 0 < size );
	_stream.read( (char*) data, size );
}

BinaryFileWriter::BinaryFileWriter( const String& filename, Mode mode ){
	if( mode == Create ){
		if( File::Exists( filename ) )
			othrow( "File already exists" );
	} else if( mode == Overwrite ){
		if( File::Exists( filename ) == false )
			othrow( "File not exists" );
	} else {
		assert( false );
	}
	_filename = filename;
	_tmpfilename = _filename + ".tmp";
	_stream.open( _tmpfilename.asStd(), std::ios::out | std::ios::trunc | std::ios::binary );
	assertStreamValid( _stream, "open binary file for write failed" );
}
BinaryFileWriter::~BinaryFileWriter(){
	if( !_closed ){
		logError( "BinaryFileWriter not closed", _filename );
		_stream.close();
	}
}
void BinaryFileWriter::write_uint8( uint8_t value ){
	_stream.write( (char*) &value, sizeof( value ) );
}
void BinaryFileWriter::write_uint32( uint32_t value ){
	_stream.write( (char*) &value, sizeof( value ) );
}
void BinaryFileWriter::write_float( float value ){
	_stream.write( (char*) &value, sizeof( value ) );
}
void BinaryFileWriter::write_bool( bool value ){
	if( value ){
		write_uint8( 1 );
	} else {
		write_uint8( 0 );
	}
}
void BinaryFileWriter::write_string( const String& value ){
	assert( value.length() < 250 );
	write_uint32( value.length() );
	_stream.write( value.asStd().data(), value.length() );
}
void BinaryFileWriter::write_vec3( const glm::vec3& value ){
	write_float( value[0] );
	write_float( value[1] );
	write_float( value[2] );
}
void BinaryFileWriter::write_vec4( const glm::vec4& value ){
	write_float( value[0] );
	write_float( value[1] );
	write_float( value[2] );
	write_float( value[3] );
}
//void BinaryFileWriter::write_vec3( const Vec3& value ){
//	write_float( value.x() );
//	write_float( value.y() );
//	write_float( value.z() );
//}
//void BinaryFileWriter::write_quat(const glm::quat& value){
//	write_float( value.x );
//	write_float( value.y );
//	write_float( value.z );
//	write_float( value.w );
//}
void BinaryFileWriter::write_block( const void* data, size_t size ){
	assert( data );
	assert( 0 < size );
	_stream.write( (const char*) data, size );
}
void BinaryFileWriter::close(){
	_stream.close();
	File::Move( _tmpfilename, _filename );
	_closed = true;
}


