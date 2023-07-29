#include "PhysicsCore.h"
#include "OdtPhysics.h"

#include "math/PosOri.h"


// STL includes
//#include <iostream>
//#include <cstdarg>
//#include <thread>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
//JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
//using namespace JPH;
// We're also using STL classes in this example
//using namespace std;

void setPosOriFromJPH( PosOri& posori, JPH::Mat44 mat44 );
void setMat4FromJPH( Mat4& t, JPH::Mat44 mat44 );

