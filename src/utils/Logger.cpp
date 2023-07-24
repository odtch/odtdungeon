#include "stdodt.h"
#include "Logger.h"

namespace OdtCore
{

	void Logger::Begin( std::ostream& stream, Level level ){
		switch( level ){
		case Error:
			stream << "\033\e[0;91merror: ";
			break;
		case Warning:
			stream << "\033\e[0;93mwarn : ";
			break;
		case Info:
			stream << "\033\e[0;96minfo : ";
			break;
		case Debug:
			stream << "\033\e[0;95mdebug: ";
			break;
		default:
			stream << "\033\e[0;91m?????: ";
			break;
		}
	}
	void Logger::End( std::ostream& stream ){
		stream << "\033[0m\n";
		stream.flush();
	}
}
