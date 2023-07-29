#pragma once

#include "ResourceCore.h"

class ResourceReader : public BinaryFileReader {
private:
	ResourceTypes* _types = null;
private:
	List<Resource*> _resources;
public:
	explicit ResourceReader( const String& filename );
	virtual ~ResourceReader() override;
public:
	Resource* read_resource();
public:
	Resource* read_reference();
};

class ResourceWriter : public BinaryFileWriter {
private:
	List<Resource*> _resources;
public:
	explicit ResourceWriter( const String& filename );
	virtual ~ResourceWriter() override;
public:
	void write( Resource* resource );
public:
	void write_reference( Resource* resource );
};
