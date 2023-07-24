#pragma GCC system_header
#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
//	#include <algorithm>
#include <list>
#include <unordered_map>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>
//	#include <cstdlib>
//	#include <cstdint>
//	#include <array>
//	#include <optional>
//	#include <set>
//#include <typeinfo>
#include <assert.h>




//#ifdef ODTVULKAN
//	#define GLFW_INCLUDE_VULKAN
//	#define VK_ENABLE_BETA_EXTENSIONS
//	#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
//	#include <vulkan/vulkan_beta.h>

////	#define GLM_FORCE_RADIANS
////	#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/glm.hpp>
#include <glm/ext.hpp>
//	#include <glm/gtc/type_ptr.hpp>
//	#include <glm/gtc/matrix_transform.hpp>
//	#include <glm/gtc/matrix_inverse.hpp>
//	#include <glm/gtx/quaternion.hpp>
//	#include <glm/gtx/hash.hpp>
//	#include <glm/gtx/string_cast.hpp>

#ifdef ODTJOLT
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseQuery.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/PhysicsMaterial.h>
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Constraints/FixedConstraint.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Vehicle/VehicleCollisionTester.h>
#endif

//#define iif( cond, a, b )( cond ? a : b )


#define null nullptr

// ich ändere das Standard-Verhalten von assert, das loggt, breakt und eine Exception wirft. ASSERT wirft keine Exception und kann daher in Destruktoren verwendet werden.
#undef assert
#undef ASSERT
#ifdef ODTDEBUG
	#define assert( expr )\
		if( static_cast<bool>( expr ) ){ \
		} else { \
			std::cerr << "assert fail " << #expr << " " << __FILE__ << " " << __LINE__ << "\n"; \
			std::cerr.flush(); \
			__assert_fail( #expr, __FILE__, __LINE__, __ASSERT_FUNCTION ); \
			throw std::runtime_error( "Assertion failed" ); \
		}
	#define ASSERT( expr ) \
		if( static_cast<bool>( expr ) ){ \
		} else { \
			std::cerr << "assert fail " << #expr << " " << __FILE__ << " " << __LINE__ << "\n"; \
			std::cerr.flush(); \
			__assert_fail( #expr, __FILE__, __LINE__, __ASSERT_FUNCTION ); \
		}
#else
	#define assert( expr )\
		if( static_cast<bool>( expr ) ){ \
		} else { \
			std::cerr << "assert fail " << #expr << " " << __FILE__ << " " << __LINE__ << "\n"; \
			std::cerr.flush(); \
			throw std::runtime_error( "Assertion failed" ); \
		}
	#define ASSERT( exp ) (void)0
#endif
template< typename T > T asserted( T value ){
	assert( value );
	return value;
}

#define ODTNOCOPY( CLAZZ ) \
	private: \
		CLAZZ( const CLAZZ& copy ) = delete; \
		void operator=( const CLAZZ& copy ) = delete;

#define odelete( obj ) { auto odeltmp = obj; obj = null; if( odeltmp )delete odeltmp; }
#define othrow( txt ) { \
	std::cerr << "throw " << txt << " " << __FILE__ << " " << __LINE__ << "\n"; \
	throw std::runtime_error( txt ); }
