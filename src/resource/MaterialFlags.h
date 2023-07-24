#ifndef MATERIALFLAGS_H
#define MATERIALFLAGS_H

const int MaterialFlag_CalcNormalFromTriangle	= 1 << 0;
const int MaterialFlag_UseNormalAsColor			= 1 << 1;
const int MaterialFlag_NoLightAffected			= 1 << 2;
const int MaterialFlag_UseTextureAsMaterial		= 1 << 3;
const int MaterialFlag_Volumetric				= 1 << 4;
const int MaterialFlag_NoShadowEmit				= 1 << 5;
const int MaterialFlag_UseTextCoordYasPower		= 1 << 6;
const int MaterialFlag_ViewOrentationToAlpha	= 1 << 7;

#endif
