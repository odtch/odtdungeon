#pragma once

#include "stdodt.h"

// inspired by https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c/69517392#69517392

namespace OdtCore
{
	class Logger {
	public:
		enum Level { Error, Warning, Info, Debug };
	public:
		template <class Args> static void print_args( std::ostream& stream, Args args ){
			stream << args << " ";
		}
		template <class... Args> static void Log( std::ostream& stream, Level level, Args... args ){
			Begin( stream, level );
			int dummy[] = {0, ((void)print_args( stream, std::forward<Args>(args)), 0)...};
			End( stream );
			if( level == Error || level == Warning ){
				stream.flush();
			}
		}
	private:
		static void Begin( std::ostream& stream, Level level );
		static void End( std::ostream& stream );
	};

}

#define logError( args... ) OdtCore::Logger::Log( std::cout, OdtCore::Logger::Error, args )
#define logWarning( args... ) OdtCore::Logger::Log( std::cout, OdtCore::Logger::Warning, args )
#define logInfo( args... ) OdtCore::Logger::Log( std::cout, OdtCore::Logger::Info, args )
#ifdef ODTDEBUG
	#define logDebug( args... ) OdtCore::Logger::Log( std::cout, OdtCore::Logger::Debug, args )
#else
	#define logDebug( args... ) (void)0
#endif

