#include "stdodt.h"
#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "utils/Module.h"
#include "utils/Clock.h"
#include "window/Window.h"

int main( int argc, char** argv ) {
    logInfo( "odtdungeon" );
	int rc;
	try {
        //		srand( time( NULL ) );
        //		//String exepath( argv[ 0 ] );
        //		String binpath( std::filesystem::path( argv[ 0 ] ).parent_path() );
        //#ifdef ODTDEBUG
        //		Converter::Run( "/home/rt/odtspace/media", binmediapath  );
        //#endif
        {
            Modules modules;
            modules.start();
            Window window;
            window.create( 800, 600, String( "ODT-Dungeon" ));
            window.main();
            window.destroy();
            rc = EXIT_SUCCESS;
            modules.stop();
        }
        Singleton::Shutdown();
        PerformanceCounter::Destroy();
#ifdef ODTDEBUG
        OdtCore::DebugCounter::Shutdown();
#endif
	} catch( const std::exception& ex ){
        logError( "main failed:", ex.what() );
        rc = EXIT_FAILURE;
	} catch( ... ){
        logError( "main failed unexpected" );
		rc = EXIT_FAILURE;
	}
    logDebug( "main.exiting rc:", rc );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) ); // qtc ist mit dem stdout verzögert
    logDebug( "main.exit rc:", rc );
	return rc;
}

