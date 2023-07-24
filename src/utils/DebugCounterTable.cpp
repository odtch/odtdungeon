#include "stdodt.h"
#include "DebugCounterTable.h"
#include "Logger.h"

#ifdef ODTDEBUG

DebugCounterTable::DebugCounterTable( const String& name )
	:_name( name )
{
}
DebugCounterTable::~DebugCounterTable(){
	_name_to_entry.clear();
	_entries.deleteAll();
}
void DebugCounterTable::inc( const String& name ){
	get( name )->counter++;
	_total_count++;
	if( 100 < _total_count ){
		traceLog();
	}
}
void DebugCounterTable::decr(const String& name){
	auto entry = get( name );
	ASSERT( 0 < entry->counter );
	entry->counter--;
	_total_count--;
}
void DebugCounterTable::traceLog(){
	logDebug( "DebugCounterTable", _name );
	for( auto entry : _entries ){
		logDebug( "  ", entry->name, entry->counter );
	}
}
DebugCounterTable::Entry* DebugCounterTable::get(const String& name){
	auto entry = _name_to_entry.find( name.asStd() );
	if( entry == null ){
		entry = new Entry();
		entry->name = name;
		_entries.add( entry );
		_name_to_entry.add( entry->name.asStd(), entry );
	}
	return entry;
}


#endif
