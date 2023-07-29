#include "stdodt.h"
#include "utils/Debugging.h"
#include "utils/Logger.h"
#include "utils/Module.h"
#include "utils/Clock.h"
#include "resource/ResourceType.h"
#include "window/Window.h"
#include "vulkan/raytracer/VulkanRaytracer.h"
#include "dungeon/DungeonCollection.h"
#include "dungeon/DungeonScene.h"
#include "dungeon/DungeonWindow.h"
#include "converter/Converter.h"
#include "physics/Physics.h"
#include "physics/PhysicsWorld.h"

int main( int argc, char** argv ) {
    logInfo( "odtdungeon" );
	int rc;
	try {
		srand( time( NULL ) );
		assert( 0 < argc );
		//String exepath( argv[ 0 ] );
		//String binpath( std::filesystem::path( argv[ 0 ] ).parent_path() );
		//logDebug( exepath ); //, binpath );
		#ifdef ODTDEBUG
				Converter::Run();
		#endif
        {
            Modules modules;
			new ResourceTypes();
			Physics physics;
            modules.start();
			new Collections();
			new DungeonCollection();
			Collections::Get()->load( String( "media" ) );
			DungeonWindow window;
			window.create( 1600, 1200, String( "ODT-Dungeon" ));
            VulkanRaytracer renderer( &window );
            renderer.start();
            DungeonScene scene( &renderer );
			window.setScene( &scene );
			new PhysicsWorld( &physics, &scene );
            scene.start();
            window.main();
            renderer.requestStop();
            scene.requestStop();
            renderer.stop();
            window.destroy();
            scene.stop();;
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

