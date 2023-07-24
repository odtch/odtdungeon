#pragma once

#include "stdodt.h"
#ifdef ODTDEBUG
#include "String.h"
#include "LookupTable.h"
#include "List.h"

class DebugCounterTable {
private:
	String _name;
	int _total_count = 0;
	struct Entry {
		String name;
		int counter = 0;
	};
	List<Entry*> _entries;
	LookupTable<std::string,Entry*> _name_to_entry;
public:
	DebugCounterTable( const String& name );
	~DebugCounterTable();
public:
	void inc( const String& name );
	void decr( const String& name );
public:
	void traceLog();
private:
	Entry* get( const String& name );
};

#else

#endif

