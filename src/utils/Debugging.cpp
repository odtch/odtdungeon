#include "stdodt.h"
#include "Debugging.h"
#include "LookupTable.h"
#include "Threading.h"

#ifdef ODTDEBUG

// NOT YET THREAD-SAFE!

namespace OdtCore
{

	class DebugCounterEntry{
	private:
		const char* _name;
		std::atomic<int> _counter;
	public:
		DebugCounterEntry( const char* name );
		~DebugCounterEntry();
	public:
		const char* name() const{ return _name; }
	public:
		void inc(){ _counter++; }
		void dec(){ _counter--; ASSERT( _counter >= 0 ); }
		int count() const{ return _counter; }
	};
	DebugCounterEntry::DebugCounterEntry( const char* name )
		:_name( name )
		,_counter( 0 )
	{
		ASSERT( _name );
	}
	DebugCounterEntry::~DebugCounterEntry(){
	}

	class DebugCounterSystem {
	public:
		static DebugCounterSystem Global;
	private:
		std::timed_mutex _mutex;
		std::unordered_map<const char*, DebugCounterEntry*> _entries;
	public:
		 DebugCounterSystem();
		 ~DebugCounterSystem();
	public:
		 void inc( const char* name );
		 void dec( const char* name );
		 void shutdown();
	private:
		 DebugCounterEntry* unsafeGetEntry( const char* name );
	};
	DebugCounterSystem DebugCounterSystem::Global;
	DebugCounterSystem::DebugCounterSystem(){
	}
	DebugCounterSystem::~DebugCounterSystem(){
	}
	void DebugCounterSystem::inc( const char* name ){
		MutexLocker locker;
		locker.lock( _mutex );
		unsafeGetEntry( name )->inc();
	}
	void DebugCounterSystem::dec( const char* name ){
		MutexLocker locker;
		locker.lock( _mutex );
		unsafeGetEntry( name )->dec();
	}
	DebugCounterEntry *DebugCounterSystem::unsafeGetEntry( const char* name ){
		ASSERT( name );
		DebugCounterEntry* entry = _entries[ name ];
		if( entry == null ){
			entry = new DebugCounterEntry( name );
			_entries[ name ] = entry;
		}
		return entry;
	}
	void DebugCounterSystem::shutdown(){
		MutexLocker locker;
		locker.lock( _mutex );
		int invalid_count = 0;
		for( auto it : _entries ){
			DebugCounterEntry* entry = it.second;
			if( 0 != entry->count() ){
				if( invalid_count == 0 ){
					std::cout << "OdtCore::DebugCounterSystem detects memory-leak:\n";
				}
				invalid_count++;
				std::cout << "  " << entry->count()
						  << " " << entry->name()
						  << "\n";
			}
		}
		if( 0 < invalid_count ){
			std::cout << "  " << invalid_count << " objects-types with memory-leaks\n";
		}
	}

	void DebugCounter::Shutdown()	{
		DebugCounterSystem::Global.shutdown();
	}
	DebugCounter::DebugCounter( const char* name )
		:_name( name ){
		DebugCounterSystem::Global.inc( _name );
	}

	DebugCounter::~DebugCounter(){
		DebugCounterSystem::Global.dec( _name );
	}



}

#endif
