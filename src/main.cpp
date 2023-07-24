#include "stdodt.h"
#include "utils/Debugging.h"
#include "utils/Logger.h"


int main( int argc, char** argv ) {
    logInfo( "odtdungeon" );
	int rc;
	try {
//		srand( time( NULL ) );

//		//String exepath( argv[ 0 ] );
//		//logDebug( "exe", exepath );
//		String binpath( std::filesystem::path( argv[ 0 ] ).parent_path() );
//		String binmediapath = binpath + "/media";
//		//logDebug( "xx", binpath );
///*
//		float s = 0.01f;
//		Vec3 t( 0, 3000, 0 );
//		Mat4 m = Mat4::Translation( t ) * Mat4::Scale( s );
//		Vec3 i( 100, 100, 100 );
//		Vec3 e = i * s + t;
//		Vec3 r = m.map( i );
//		logDebug( "u", i, "e: ", e, "r: ", r, "error", ( e - r ).length() );
//		assert( false );
//*/
////	Json::Test();
///* */
//		new Modules();

//		App* app = App::Create();
//		assert( app );
//		app->onStartup();
//#ifdef ODTDEBUG
//		Converter::Run( "/home/rt/odtspace/media", binmediapath  );
//		//assert( false );
//#endif
//		Collections::Get()->load( binmediapath );
//		app->main( binpath );
//		Modules::Get()->onShutdown();
//		odelete( app );
///* */
        rc = EXIT_SUCCESS;
//		Singleton::Shutdown();
//		PerformanceCounter::Destroy();
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
	std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    logDebug( "main.exit rc:", rc );
	return rc;
}

