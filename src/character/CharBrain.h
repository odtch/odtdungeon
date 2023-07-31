#pragma once

#include "CharCore.h"
#include "brain/BrainCore.h"

class CharBrain
{
public:
	static uint InputSize();
	static uint OutputSize();
	static void LoadInput( FloatBuffer& input, float control_move_y, const CharPose& pose );
	static void LoadOutput( FloatBuffer& output, const CharPose& pose );
	static void LoadOutput( CharPose& pose, const FloatBuffer& output );
};
