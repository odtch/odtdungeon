#include "stdodt.h"
#ifdef ODTDEBUG
#include "Converter.h"
#include "utils/File.h"
#include "utils/Logger.h"
//#include "resource/Resources.h"
//#include "resource/MeshModel.h"
//#include "resource/Texture.h"
#include "resource/Image.h"
#include "resource/Skeleton.h"
#include "AssImp.h"
#include "converter/AssImpAnimation.h"
#include "character/CharImporter.h"
#include "character/CharRagdoll.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"
#include <filesystem>

//#define REBUILD_ALL

void Converter::Run(){
	Converter converter;
	converter.run();
}
Converter::Converter(){
	_sourcepath = String( std::filesystem::current_path() );
	_targetpath = String( "media" );
}
Converter::~Converter(){
}
void Converter::run(){
	if( _sourcepath.endsWith( String( "build" ) ) ){
		logError( "sourcepath", _sourcepath );
		throw std::runtime_error( "the working-directory should be the root of the project, not the build-directory" );
	}
	File::CreateDirectoryIfNotExists( _targetpath );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.rgen.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.rahit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.rchit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.rmiss.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.shadow.rahit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.shadow.rchit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.shadow.rmiss.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.transl.rahit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.transl.rchit.glsl" );
	compileShader( "1.2", "src/vulkan/raytracer.glsl/vrt.transl.rmiss.glsl" );
	convertImage( "test_red", "/home/rt/media/test/TestRed.png" );
	convertImage( "mcg_diff", "/home/rt/media/mocap/FBX_Ninja_v27_Pro/MotusMan_v55/MotusMan_v55.fbm/MCG_diff.jpg" );
	if( shouldConvert( String( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx" ), _targetpath + "/motusman_ragdoll" ) == true ){
		CharImporter charimporter( CharImporter::MocapFormat );
		charimporter.createRagdoll();
		AssImp assimp;
		assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx", AssImp::YUp_to_ZUp_Synty2() );
		Skeleton* skeleton = assimp.loadSkeleton();
		charimporter.setupRagdollFromSkeleton( *skeleton );
		charimporter.loadSkin( *skeleton, assimp, 0 );
		odelete( skeleton );
		CharRagdollType* ragdolltype = charimporter.ragdolltype();
		BinaryFileWriter writer( _targetpath + "/motusman_ragdoll" );
		ragdolltype->save( writer );
		writer.close();
		odelete( ragdolltype );
	}
	//material->setFlag( MaterialFlag_CalcNormalFromTriangle );
//	{
//		AssImp assimp;
//		assimp.open( "/home/rt/media/mocap/MotusMan_v55/MotusMan_v55.fbx", AssImp::YUp_to_ZUp_Synty2() );
//		assimp.trace();
//		//assimp.l
//		Skeleton* skeleton = assimp.loadSkeleton();
////		charimporter.setupRagdollFromSkeleton( *skeleton );
////		charimporter.loadSkin( *skeleton, assimp, 0 );
//		//skeleton->trace();
//		odelete( skeleton );
////		addResource( "MotusMan", charimporter.ragdolltype() );
	//	}
}
#ifdef ooold

	//compileShader( "1.1", "space/vulkan/rasterizer/VulkanRasterizerPNT.frag.glsl" );
	//compileShader( "1.1", "space/vulkan/rasterizer/VulkanRasterizerPNT.vert.glsl" );
	//compileShader( "1.2", "space/vulkan/raycompute/VulkanComputeRaytracer.comp.glsl" );

	compileShader( "1.2", "src/skin/skinmesher.comp.glsl" );

	//compileShader( "1.2", "space/vulkan/raytracer/sensorshader/VulkanSensorRaytracer.rgen.glsl" );
	//compileShader( "1.2", "space/vulkan/raytracer/sensorshader/VulkanSensorRaytracer.rmiss.glsl" );
	//compileShader( "1.2", "space/vulkan/raytracer/sensorshader/VulkanSensorRaytracer.rchit.glsl" );

	convertImage( _targetpath + "/ui", "/home/rt/media/fonts/Aldrich.png" );
	copy( _targetpath + "/uiofi", "/home/rt/media/fonts/Aldrich.ofi" );


	// makes the fbxinfo
	//convertCollection( "RtsCollection", "/home/rt/media/RtsCollection" );
	//convertCollection( "KriptoMuzzle", "/home/rt/media/KriptoFX/MuzzleFlashes/" );
	//convertCollection( "KriptoRealisticEffectsV4", "/home/rt/media/KriptoFX/RealisticEffectsV4/" );
	//convertCollection( "SyntyNature", "/home/rt/media/Polygon_Nature" );
	//convertCollection( "SyntyNatureSwamp", "/home/rt/media/Polygon_Nature_Swamp/src" );
	//convertCollection( "SyntyScifiWorlds", "/home/rt/media/Polygon_SciFi_Worlds" );

	convertSrcMedia();
	//File::DeleteFileIfExists( _media + "/SM_Env_Tree_Birch_01.fbx.info" );
	//File::DeleteFileIfExists( _binmedia + "/SyntyNatureMeadow" );
//	buildCollectionTemplate( "/home/rt/media/Polygon_Nature_Tropical/Polygon_Nature_Tropical.template.json", "Polygon_Nature_Tropical", "/home/rt/media/Polygon_Nature_Tropical", _targetpath );

	//convertCollection( "SyntyNatureMeadow", "/home/rt/media/Polygon_Nature_Meadow/src" );
	//convertCollection( "SyntyScifiWorlds", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles" );

	/*
File::DeleteFileIfExists( "build/media/syn_nt_SM_Tree_Pine_01" );
	addTexture( "test_blue", "/home/rt/media/test/TestBlue.png" );
	addTexture( "test_green", "/home/rt/media/test/TestGreen.png" );
	addTexture( "test_red", "/home/rt/media/test/TestRed.png" );
	addTexture( "test_grid_colored", "/home/rt/media/test/TestGridColored.png" );
	addTexture( "test_grid_colored2", "/home/rt/media/test/TestGridColored2.png" );
	addTexture( "syn_sp_sky01b", "/home/rt/media/Polygon_SciFi_Space/Textures/PolygonSciFiSpace_Skybox_01_Back.png" );
	addTexture( "island", "/home/rt/Downloads/island.png" );
	addTexture( "heightmap01", "/home/rt/media/unsorted/heightmap01.png" );

	addTexture( "jupiter", "/home/rt/media/odtspace/jupiter.png" );
	addTexture( "neptune", "/home/rt/media/odtspace/neptune.png" );
	addTexture( "sun", "/home/rt/media/odtspace/sun.png" );





	addTexture( "syn_sw_01a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_01_A.png" );
	addTexture( "syn_sw_01b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/.png" );
	addTexture( "syn_sw_01c", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_01_C.png" );
	addTexture( "syn_sw_02a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_02_A.png" );
	addTexture( "syn_sw_02b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_02_B.png" );
	addTexture( "syn_sw_02c", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_02_C.png" );
	addTexture( "syn_sw_03a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_03_A.png" );
	addTexture( "syn_sw_03b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_03_B.png" );
	addTexture( "syn_sw_03c", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_03_C.png" );
	addTexture( "syn_sw_04a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_04_A.png" );
	addTexture( "syn_sw_04b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_04_B.png" );
	addTexture( "syn_sw_04c", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_04_C.png" );
	addTexture( "syn_sw_s01f", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/.png" );
	addTexture( "syn_sw_s01l", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/" );
	addTexture( "syn_sw_s01u", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/.png" );
	addTexture( "syn_sw_s01b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/.png" );
	addTexture( "syn_sw_s01r", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/.png" );
	addTexture( "syn_sw_s01d", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/.png" );
	convertMeshFile( "syn_sw_buggy", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/FBX/SM_Veh_Buggy_01.fbx" );

	//convertMeshFile( "syn_sw_c", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/FBX/Characters.fbx" );
	convertMeshFile( "syn_ss_c", "/home/rt/media/Polygon_SciFi_Space/Characters/SK_Chr_CrewCaptain_Female_01.fbx" );
	addTexture( "syn_ss_01a", "/home/rt/media/Polygon_SciFi_Space/Textures/PolygonSciFiSpace_Texture_01_A.png" );


	addTexture( "", "/home/rt/media/Polygon_Nature/Textures/" );
	addTexture( "syn_nt_leaves_pine_texture", "/home/rt/media/Polygon_Nature/Textures/" );
	addTexture( "syn_nt_leaves_pine_snow_texture", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_basegrass_normals_texture", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_leaves_genereric", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_leaves_dead", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_undergrowth", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_fern", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_flowerpatch", "/home/rt/media/Polygon_Nature/Textures/.png" );
	addTexture( "syn_nt_birch_trunk", "/home/rt/media/Polygon_Nature/Textures/" );


	convertMeshFile( "syn_nt_t01", "/home/rt/media/Polygon_Nature/FBX/SM_Tree_Large_01.fbx" );
	convertMeshFile( "syn_nt_t02", "/home/rt/media/Polygon_Nature/FBX/SM_Tree_PolyPine_01.fbx" );
	convertMeshFile( "syn_nt_t03", "/home/rt/media/Polygon_Nature/FBX/SM_Tree_Round_01.fbx" );
	convertMeshFile( "syn_nt_t04", "/home/rt/media/Polygon_Nature/FBX/SM_Tree_Round_04.fbx" );
	//convertMeshFile( "syn_nt_t02", "/home/rt/media/Polygon_Nature/FBX/SM_Tree_Willow_Small_01.fbx" );

	{
		String collection_prefix = "syn_nt_";
		File::Find( "/home/rt/media/Polygon_Nature/FBX", ".fbx", [this,&collection_prefix]( const File::Entry& entry ){
			//logDebug( "u", entry.name , "\t", entry.path );
			convertMeshFile( collection_prefix + entry.name, entry.path );
		} );
	}
//assert( false );


	addTexture( "syn_sw_01a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_01_A.png" );
	addTexture( "syn_sw_01b", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_01_B.png" );
	addTexture( "syn_sw_02a", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/Textures/Alts/PolygonScifiWorlds_Texture_02_A.png" );
	convertMeshFile( "syn_sw_dropship", "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/FBX/.fbx" );

	if( File::Exists( media + "/log.txt" ) )
		File::DeleteFile( media + "/log.txt" );
	File::CreateText( media + "/log.txt", debug_models );
	*/

}
/*
void Converter::convertMeshFile( const String& id, const String& filename){
	if( !isModified( id, filename ) )
		return;
	String fn = this->filename( id );
	//if( File::Exists( fn ) )return;
	AssImp assimp;
	assimp.open( filename.asStd() );
	assimp.trace( std::cout );
	assimp._transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
//int todooo;assert( false );
	debug_models += id;
	debug_models += "\n";
	MeshModel* model = assimp.loadModel();
	assert( model );
	for( MeshModelNode* node : model->allNodes() ){
		for( MeshModelMatMesh* matmesh : node->meshes() ){
			MeshPNT* mesh = matmesh->mesh();
			debug_models += "\tm: ";
			debug_models += node->name();
			debug_models += "\n";
			debug_models += "\t\tm: ";
			debug_models += mesh->name();
			debug_models += "\n";
			debug_models += "z: ";
			debug_models += mesh->name();
			debug_models += "\t";
			debug_models += model->name();
			debug_models += "\n";
		}
	}
#ifdef ODTDEBUG
	Resources::SaveMeshModel( fn, *model );
#else
	 assert( false );
#endif
	delete model;
}
*/
#endif
bool Converter::shouldConvert( const String& srcpath, const String& trgpath ){
	if( File::Exists( trgpath ) == false )
		return true;
	auto srctime = std::filesystem::last_write_time( srcpath.asStd() );
	auto trgtime = std::filesystem::last_write_time( trgpath.asStd() );
	bool src_is_newer = ( srctime > trgtime );
#ifdef REBUILD_ALL
	src_is_newer = true;
#endif
	if( src_is_newer ){
		File::DeleteFile( trgpath );
		return true;
	} else {
		return false;
	}
}
void Converter::compileShader( const String& vulkanLevel, const String& fn ){
	std::filesystem::path path( fn.asStd() );
	String trgpath = _targetpath + "/" + path.filename().generic_string() + ".spv";
	if( shouldConvert( fn, trgpath ) == false )
		return;
	logInfo( "compile shader", vulkanLevel, path.filename().c_str() );
    String filename( fn );
    String shader_stage;
    if( filename.contains( ".comp." ) ){
        shader_stage = "compute";
    } else if( filename.contains( ".frag." ) ){
        shader_stage = "fragment";
    } else if( filename.contains( ".vert." ) ){
        shader_stage = "vertex";
    } else if( filename.contains( ".rgen." ) ){
        shader_stage = "rgen";
	} else if( filename.contains( ".rahit." ) ){
		shader_stage = "rahit";
	} else if( filename.contains( ".rchit." ) ){
		shader_stage = "rchit";
	} else if( filename.contains( ".rmiss." ) ){
        shader_stage = "rmiss";
	} else if( filename.contains( ".rchit." ) ){
        shader_stage = "rchit";
    } else {
        logError( "unhandled shader_stage from filename" );
        assert( false );
    }
	String cmd;
	// /home/rt/vulkan/x86_64/bin/glslangValidator --target-env vulkan1.2 -o /home/rt/temp/space/raytrace.rgen.glsl.spv space/shader/raytrace.rgen.glsl
// 	cmd += "glslangValidator ";
// 	cmd += " --target-env vulkan";
// 	cmd += vulkanLevel;
// 	cmd += " ";
// 	cmd += "-o ";
// 	cmd += media + "/";
// 	cmd += path.filename().c_str();
// 	cmd += ".spv ";
// 	cmd += fn;
    // /home/rt/vulkan/x86_64/bin/glslc --target-env=vulkan1.2 -I space/vulkan/shader -fshader-stage=compute -o /home/rt/temp/space/raytracer.comp.glsl.spv space/vulkan/shader/raytracer.comp.glsl
    cmd += "glslc --target-env=vulkan";
    cmd += vulkanLevel;
	cmd += " -I src";
    cmd += " -fshader-stage="; //compute
    cmd += shader_stage;
    cmd += " -o ";
	cmd += _targetpath + "/";
 	cmd += path.filename().c_str();
 	cmd += ".spv ";
 	cmd += fn;
    logDebug( cmd );
	int rc = std::system( cmd.asCStr() );
	if( rc != 0 ){
		logError( "std::system failed", rc, cmd );
		assert( false );
	}
}
float i16toc( stbi_us i16 ){
	// stbi_us = unsigned short = 0 to 65535
	float f = i16;
	f /= 65535;
	ASSERT( 0 <= f && f <= 1 );
	return f;
}
void Converter::convertImage( const String& trgname, const String& srcpath ){
	String trgpath = _targetpath + "/" + trgname;
	if( File::Exists( trgpath ) )
		return;
	logInfo( "converting image", srcpath, trgpath );
	int texWidth, texHeight, texChannels;
	//stbi_uc* pixels = stbi_load( srcpath.asCStr(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
	stbi_us* pixels = stbi_load_16( srcpath.asCStr(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
	stbi_us* rgba = pixels;
	if( !pixels ){
		logError( "load image failed", srcpath );
		assert( false );
	}
	Image image;
	image.create( texWidth, texHeight );
	for( uint y = 0; y < texHeight; y++ ){
		for( uint x = 0; x < texWidth; x++ ){
			float r = i16toc( *rgba ); rgba++;
			float g = i16toc( *rgba ); rgba++;
			float b = i16toc( *rgba ); rgba++;
			float a = i16toc( *rgba ); rgba++;
			glm::vec4 c( r, g, b, a );
			image.set( x, y, c );
		}
	}
	BinaryFileWriter writer( trgpath );
	image.save( writer );
//	{
//		BinaryFileWriter writer( trgpath );
//		writer.write_uint32( texWidth );
//		writer.write_uint32( texHeight );
//		int pixel_size = texWidth * texHeight * 4;
//		writer.write_uint32( pixel_size );
//		writer.write_block( pixels, pixel_size );
//		writer.close();
//	}
	writer.close();
	stbi_image_free( pixels );
}


#ifdef old
void Converter::convertMesh( const String& name, const String& srcpath, const Mat4& transform ){
	String trgpath = _targetpath + "/" + name;
#ifndef REBUILD_ALL
	if( File::Exists( trgpath ) )
		return;
	AssImp assimp;
	assimp.open( srcpath, transform );
	assimp.trace();
	MeshPNT* mesh = assimp.loadMeshPNT( 0 );
	//MeshModel* model = assimp.loadModel();
	BinaryFileWriter writer( trgpath );
	mesh->save( writer );
	writer.close();
	std::cout << "m" <<  mesh->calcBoundingBox();
	odelete( mesh );
#endif
}
void Converter::copy( const String& trgpath, const String& srcpath ){
	if( File::Exists( trgpath ) )
		return;
	String text = File::LoadText( srcpath );
	File::CreateText( trgpath, text );
}
void Converter::convertCollection( const String& id, const String& srcpath ){
	String bincollpath = _targetpath + "/" + id;
	File::CreateDirectoryIfNotExists( bincollpath );
	buildCollectionTemplate( String( "media/" ) + id + ".template.json", id, srcpath, bincollpath );
	//String collectionJsonFile =
}
void Converter::buildCollectionTemplate( const String& trgpath, const String& id, const String& srcpath, const String& bincollpath ){
	//File::DeleteFileIfExists( trgpath );
#ifdef REBUILD_ALL
#else
	if( File::Exists( trgpath ) )
		return;
#endif
	File::CreateDirectoryIfNotExists( bincollpath + "/fbx.info" );
	std::stringstream collectiontemplate;
	collectiontemplate << "{\n";
	collectiontemplate << "\t\"id\": \"" << id << "\",\n";
	collectiontemplate << "\t\"src\": \"" << srcpath << "\",\n";
	List<File::Entry*> files;
	File::GetFiles( srcpath, File::FindOptions( true ), files );
	File::SortFiles( files );
	{
		collectiontemplate << "\t\"textures\": [";
		bool first = true;
		for( File::Entry* file : files ){
			if( file->path.endsWith( ".png" ) || file->path.endsWith( ".tga" ) ){
				if( first ){ first = false; } else { collectiontemplate << ","; }
				collectiontemplate << "\n\t\t{ \"id\": \"" << file->name << "\"" << String::Repeated( " ", ( file->name.length() < 50 ? 50 - file->name.length() : 0 ) ) << "\t, \"file\": \"" << file->path << "\" }";
			}
		}
		collectiontemplate << "\n\t],\n";
	}{
		collectiontemplate << "\t\"models\": [";
		bool first = true;
		for( File::Entry* file : files ){
			if( file->path.endsWith( ".fbx" ) ){
				if( first ){ first = false; } else { collectiontemplate << ","; }
				collectiontemplate << "\n\t\t{ \"id\": \"" << file->name << "\"" << String::Repeated( " ", ( file->name.length() < 50 ? 50 - file->name.length() : 0 ) ) << "\t, \"file\": \"" << file->path << "\"";
				String infopath = file->name;
				infopath.replaceAll( '/', '.' );
				infopath = bincollpath + "/fbx.info/" + infopath + ".info";
				if( !File::Exists( infopath ) ){
					String info;
					AssImp* assimp = null;
					try{
						assimp = new AssImp();
						assimp->open( file->path.asStd() );
						std::stringstream ss;
						assimp->trace( ss );
						info = String( ss.str() );
						//odelete( assimp );
					} catch( const std::exception& ex ){
						logError( "Converter::buildCollectionTemplate failed:", file->path, ex.what() );
						info = String( "Error: " ) + ex.what();
					}
					File::CreateText( infopath, info );
				}
				collectiontemplate << " }";
			}
		}
		collectiontemplate << "\n\t]\n";
	}
	files.deleteAll();

	collectiontemplate << "}";
	File::CreateText( trgpath, collectiontemplate.str() );

	/*
	String info;
	{
		BinaryFileWriter writer( tmppath );
		writer.write_uint32( 134731 );
		info += "Textures\n";
		{
			List<File::Entry*> allfiles;
			File::GetFiles( srcpath + "/Textures", File::FindOptions( ".png", true ), allfiles );
			File::SortFiles( allfiles );
			List<File::Entry*> files;
			for( File::Entry* entry : allfiles ){
				bool skip = false;
				if( id == "SyntyNature" ){
					//if( entry->name.startsWidth( "Grass_Textures/" ) )skip = true;
					//if( entry->name.startsWidth( "Ground_Textures/" ) )skip = true;
					skip = true;
					if( entry->name.startsWith( "Leaves/Leaves_Willow_Texture" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Leaves_Pine_Texture" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Leaves_Pine_Snow_Texture" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Leaves_Generic_Texture" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Tree_Dead_Branch" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Undergrowth_Texture" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Fern_Texture" ) )skip = false;
					if( entry->name.startsWith( "Misc/FlowerPatch_02" ) )skip = false;
					if( entry->name.startsWith( "Leaves/Birch_Trunk_Texture" ) )skip = false;
					if( entry->name.startsWith( "Ground_Textures/BaseGrass_normals" ) )skip = false;
					if( entry->name.startsWith( "PolygonNature_0" ) )skip = false;
				}
				if( skip ){
					logInfo( "  skip image", entry->name, entry->path );
					continue;
				}
				files.add( entry );
			}
			LookupTable<std::string,File::Entry*> t;
			writer.write_uint32( files.size() );
			for( File::Entry* entry : files ){
				logInfo( "  image", entry->name, entry->path );
				info += entry->name + "\n";
				t.add( entry->name.asStd(), entry );
				writer.write_string( entry->name );
				{
					writer.write_uint8( 'I' );
					int texWidth, texHeight, texChannels;
					stbi_uc* pixels = stbi_load( entry->path.asCStr(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
					if( !pixels ){
						logError( "load image failed", entry->path );
						assert( false );
					}
					writer.write_uint32( texWidth );
					writer.write_uint32( texHeight );
					int pixel_size = texWidth * texHeight * 4;
					writer.write_uint32( pixel_size );
					writer.write_block( pixels, pixel_size );
					stbi_image_free(pixels);
					writer.write_uint8( 'D' );
				}
				writer.write_uint32( 134737 );
			}
			files.removeAll();
			allfiles.deleteAll();
		}
		writer.write_uint32( 134732 );
		info += "\nMeshes\n";
		{
			List<File::Entry*> files;
			File::GetFiles( srcpath + "/FBX", File::FindOptions( ".fbx", true ), files );
			File::SortFiles( files );
			LookupTable<std::string,File::Entry*> t;
			writer.write_uint32( files.size() );
			for( File::Entry* entry : files ){
				logInfo( "  fbx", entry->name, entry->path );
				if( entry->path == "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/FBX/BR_Characters.fbx"
					|| entry->path == "/home/rt/media/Polygon_SciFi_Worlds/_Sourcefiles/FBX/Characters.fbx"
						){
					logError( "uu" );
					int todo;
					continue;
				}
				info += entry->name + "\n";
				t.add( entry->name.asStd(), entry );
				writer.write_string( entry->name );
				{
					AssImp assimp;
					assimp.open( entry->path.asStd() );
					std::stringstream ss;
					assimp.trace( ss );
					ss
					assimp._transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
					MeshModel* model = assimp.loadModel();
					assert( model );
					DictionaryVariant json;
					json.set( "name", model->name() );
					{
						ListVariant* jsonmeshs = new ListVariant(); json.add( "meshes", jsonmeshs );
						for( MeshPNT* mesh : model->meshes() ){
							DictionaryVariant* jsonnodemesh = new DictionaryVariant(); jsonmeshs->add( jsonnodemesh );
							jsonnodemesh->set( "name", mesh->name() );
						}

					}
					ListVariant* jsonnodes = new ListVariant(); json.add( "nodes", jsonnodes );
					for( MeshModelNode* node : model->allNodes() ){
						DictionaryVariant* jsonnode = new DictionaryVariant(); jsonnodes->add( jsonnode );
						jsonnode->set( "name", node->name() );
						ListVariant* jsonnodemeshs = new ListVariant(); jsonnode->add( "meshes", jsonnodemeshs );
						for( MeshModelMatMesh* matmesh : node->meshes() ){
							DictionaryVariant* jsonnodemesh = new DictionaryVariant(); jsonnodemeshs->add( jsonnodemesh );
							jsonnodemesh->set( "mesh", matmesh->mesh()->name() );
							if( matmesh->hasMaterial() ){
								jsonnodemesh->set( "material", matmesh->material()->name() );
							} else {
								jsonnodemesh->set( "material", String() );
							}

							MeshPNT* mesh = matmesh->mesh();

						}
					}
					String jsonfilename = collpath + "/fbx." + entry->name + ".json";
					File::DeleteFileIfExists( jsonfilename );
					File::CreateText( jsonfilename, json.toJson() );
					Resources::SaveMeshModel( writer, *model );
					delete model;
				}
				writer.write_uint32( 134738 );
			}
			files.deleteAll();
		}
		writer.write_uint32( 134733 );
	}
	{
		/*
				if( id == "SyntyNature" && entry->path.startsWidth( "/home/rt/media/Polygon_Nature/Textures/Grass_Textures/Flat/" ) ){
					entry->name = String( "Flat" ) + entry->name;
				}
		 * * /
	}
	File::CreateText( trgpath + ".info", info );
	File::Move( tmppath, trgpath );

}
*/
}

template < typename T >
class ByStringList {
private:
	std::unordered_map<std::string,T> _map;
public:
	explicit ByStringList(){
	}
	~ByStringList(){
	}
public:
	void add( const String& key, T value ){
		_map[ key.asStd() ] = value;
	}
	T get( const String& key ){
		auto it = _map.find( key.asStd() );
		if( it == _map.end() ){
			logError( "key not found", key );
			assert( false );
		}
		return it->second;
	}
};
void Converter::convertSrcMedia(){
	File::Find( _srcmediapath, File::FindOptions( true ), [this]( const File::Entry& entry ){
		if( entry.path.endsWith( ".template.json" ) )
			return;
		if( !entry.path.endsWith( ".json" ) )
			return;
		//logDebug( entry.name );
		DictionaryVariant* json = Json::ParseObject( File::LoadText( entry.path ) );
		convertSrcCollection( *json );
		odelete( json );
	} );
}
void Converter::convertSrcCollection( DictionaryVariant& json ){
	assert( false );
	/*
	String collectionid = json.getString( "id" );
	String collectionFileName = _targetpath + "/" + collectionid + ".obc";
#ifdef REBUILD_ALL
	//File::DeleteFileIfExists( collectionFileName );
#endif
	if( File::Exists( collectionFileName ) )
		return;
	BinaryFileWriter writer( collectionFileName );
	writer.write_uint32( 134730 );
	List<Resource*> resourcesToDelete;
	ByStringList<int> textureid2index;
	{
		writer.write_uint8( 'T' );
		ListVariant& jsontextures = json.getList( "textures" );
		writer.write_uint32( jsontextures.size() );
		int textureindex = 0;
		for( Variant* jsontexture : jsontextures ){
			String textureid = jsontexture->asDictionary().getString( "id" );
			writer.write_string( textureid );
			String filepath = jsontexture->asDictionary().getString( "file" );
			Image image;
			image.import( filepath );
			image.write( writer );
			textureid2index.add( textureid, textureindex );
			textureindex++;
		}
	}
	ByStringList<int> materialid2index;
	{
		writer.write_uint8( 'm' );
		writer.write_uint32( json.getList( "materials" ).size() );
		int materialindex = 0;
		for( Variant* jsonmaterial : json.getList( "materials" ) ){
			String materialid = jsonmaterial->asDictionary().getString( "id" );
			writer.write_string( materialid );
			String textureid = jsonmaterial->asDictionary().getString( "texture" );
			//logDebug( textureid );
			int textureindex = textureid2index.get( textureid );
			writer.write_uint32( textureindex );
			materialid2index.add( materialid, materialindex );
			materialindex++;
		}
	}
	writer.write_uint8( 'M' );
	writer.write_uint32( json.getList( "models" ).size() );
	for( Variant* jsonmodel : json.getList( "models" ) ){
		String modelid = jsonmodel->asDictionary().getString( "id" );
		writer.write_string( modelid );
		AssImp assimp;
		assimp.open( jsonmodel->asDictionary().getString( "file" ).asStd() );
		assimp._transform = Mat4::Scale( 0.01f ) * Mat4::RotationAtAxis( -90, Vec3::Right ) * Mat4::RotationAtAxis( 180, Vec3::Forward );
		MeshModel* model = assimp.loadModel();
		assert( model );		
		if( jsonmodel->asDictionary().has( "meshes" ) == false ){
			writer.write_uint8( 'm' );
			Resources::SaveMeshModel( writer, *model );
		} else {
			writer.write_uint8( 'c' );
			ByStringList<int> meshid2index;
			{
				ListVariant& jsonmeshes = jsonmodel->asDictionary().getList( "meshes" );
				writer.write_uint32( jsonmeshes.size() );
				int meshindex = 0;
				for( Variant* jsonmesh : jsonmeshes ){
					String meshid = jsonmesh->asDictionary().getString( "id" );
					writer.write_string( meshid );
					int meshindexinassimp = jsonmesh->asDictionary().getString( "index" ).toInt();
					MeshPNT* mesh = model->meshes().get( meshindexinassimp );
					if( jsonmesh->asDictionary().has( "filter" ) ){
						String filterid = jsonmesh->asDictionary().getString( "filter" );
						MeshPNT* filtered = null;
						if( filterid == "color.b<0.5" ){
							filtered = Filtered( *mesh, [&assimp,meshindexinassimp]( int v, const VertexPNT& vertex ){
								vec4 color = assimp.getVertexColor( meshindexinassimp, v );
								return color[2] < 0.5f;
							} );
						} else if( filterid == "color.b>0.5" ){
							filtered = Filtered( *mesh, [&assimp,meshindexinassimp]( int v, const VertexPNT& vertex ){
								vec4 color = assimp.getVertexColor( meshindexinassimp, v );
								return color[2] > 0.5f;
							} );
						} else {
							assert( false );
						}
						assert( filtered );
						resourcesToDelete.add( filtered );
						mesh = filtered;
					}
					Resources::WriteMeshPNT( writer, *mesh );
					meshid2index.add( meshid, meshindex );
					meshindex++;
					logDebug( meshindex, meshid );
				}
			}{
				ListVariant& jsonvariants = jsonmodel->asDictionary().getList( "variants" );
				writer.write_uint32( jsonvariants.size() );
				for( Variant* jsonvariant : jsonvariants ){
					String variantid = jsonvariant->asDictionary().getString( "id" );
					writer.write_string( variantid );
					ListVariant& jsonnodes = jsonvariant->asDictionary().getList( "nodes" );
					writer.write_uint32( jsonnodes.size() );
					for( Variant* jsonnode : jsonnodes ){
						String variantnodeid = jsonnode->asDictionary().getString( "id" );
						writer.write_string( variantnodeid );
						String nodeid = jsonnode->asDictionary().getString( "node" );
						MeshModelNode* node = model->getNode( nodeid );
						node->absoluteMatrix().write( writer );
						String meshid = jsonnode->asDictionary().getString( "mesh" );
						int meshindex = meshid2index.get( meshid.asStd() );
						writer.write_uint32( meshindex );
						String materialName = jsonnode->asDictionary().getString( "material" );
						//logDebug( "conv", materialName );
						//if( materialName.equals( "" ))
						int matindex = materialid2index.get( materialName );
						writer.write_uint32( matindex );
					}
				}
			}
			odelete( model );
		}
	}
	writer.write_uint32( 134733 );
	resourcesToDelete.deleteAll();
	*/
}

#endif
#endif
