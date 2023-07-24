#pragma once

#include "Debugging.h"
#include "String.h"
#include "List.h"

class File;
class BinaryFileReader;
class BinaryFileWriter;

class File {
public:
	static bool Exists( const String& filename );
	static void DeleteFile( const String& filename );
	static void DeleteFileIfExists( const String& filename );
	static void Move( const String& src, const String& trg );
	static void CreateDirectory( const String& path );
	static void CreateDirectoryIfNotExists( const String& path );
	static String LoadText( const String& filename );
	static void CreateText( const String& filename, const String& content );
public:
	static String GetNameFromPath( const String& filename ); // /home/test.png = test
public:
	struct FindOptions {
		String filterDotExt;
		bool recursive = false;
	public:
		explicit FindOptions( bool recursive )
			:recursive( recursive )
		{}
		explicit FindOptions( const String& filterDotExt, bool recursive )
			:filterDotExt( filterDotExt ), recursive( recursive )
		{}
	};
	struct Entry {
		String name; // datei
		String path; // /home/test/dir/datei.txt
	public:
		explicit Entry(){}
		explicit Entry( const String& name, const String& path )
			:name( name ), path( path )
		{}
	};
	static void Find( const String& directory, const FindOptions& options, std::function<void( const Entry& entry )> handler );
	static void GetFiles( const String& directory, const FindOptions& options, List<Entry*>& files );
	static void SortFiles( List<Entry*>& files );
private:
	String _path;
public:
	explicit File();
	~File();
public:
	void create( const String& path, std::function<void( BinaryFileWriter& writer )> saver );
	void load( const String& path, std::function<void( BinaryFileReader& reader )> loader );
	void save( std::function<void( BinaryFileWriter& writer )> saver );
	bool isOpen() const;
	void close();
};

class BinaryFileReader {
	DEBUGCOUNTER(BinaryFileReader)
private:
	std::ifstream _stream;
	char _buffer[ 256 ];
public:
	explicit BinaryFileReader( const String& filename );
	virtual ~BinaryFileReader();
public:
	uint8_t read_uint8();
	uint32_t read_uint32();
	void read_magicnumber( uint32_t expected );
	float read_float();
	bool read_bool();
	void read_string( String& value );
//	void read_vec3( Vec3& value );
	glm::vec3 read_vec3();
	glm::vec4 read_vec4();
//	void read_quat( glm::quat& value );
	void read_block( void* data, size_t size );
public:
	std::ifstream& asStd(){ return _stream; }
};

class BinaryFileWriter {
	DEBUGCOUNTER(BinaryFileWriter)
public:
	enum Mode { Create, Overwrite };
private:
	String _filename;
	String _tmpfilename;
	bool _closed = false;
	std::ofstream _stream;
public:
	explicit BinaryFileWriter( const String& filename, Mode mode = Create );
	virtual ~BinaryFileWriter();
public:
	void write_uint8( uint8_t value );
	void write_uint32( uint32_t value );
	void write_float( float value );
	void write_bool( bool value );
	void write_string( const String& value );
	void write_vec3( const glm::vec3& value );
	void write_vec4( const glm::vec4& value );
//	void write_quat( const glm::quat& value );
	void write_block( const void* data, size_t size );
public:
	std::ofstream& asStd(){ return _stream; }
public:
	void close();
};
