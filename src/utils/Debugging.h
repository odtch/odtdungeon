#pragma once

#include "stdodt.h"

// better: http://funwithlinux.blogspot.com/2007/02/detecting-memory-leaks-in-gnulinux.html
// http://www.valgrind.org/
// or own: http://www.c-plusplus.de/forum/viewtopic-var-p-is-1881422.html
// respektive aris04

// Avoid "unused parameter" warnings
#define OUNUSED(x) (void)x;

#ifdef ODTDEBUG

namespace OdtCore
{
	class DebugCounter {
	public:
		static void Shutdown();
	private:
		const char* _name;
	public:
		DebugCounter( const char* name );
		~DebugCounter();
	public:
		const char* name() const{ return _name; }
	};

	#define DEBUGCOUNTER( CLAZZ ) class CLAZZ##DebugCounter : public OdtCore::DebugCounter { \
		public: \
			CLAZZ##DebugCounter() \
				:DebugCounter( #CLAZZ ){ \
			} \
		}; \
		CLAZZ##DebugCounter _debugCounter;

}

#else

	#define DEBUGCOUNTER( CLAZZ )

#endif

