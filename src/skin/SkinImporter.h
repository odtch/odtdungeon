#pragma once

#include "SkinCore.h"
#include "converter/AssImpCore.h"
//#include "SkinMesh.h"

class SkinImporter {
public:
	static SkinType* Import( AssImp& assimp, uint meshindex );
};
