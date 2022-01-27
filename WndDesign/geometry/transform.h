#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Transform {
	float matrix[3][2];

	static Transform Identity();
	static Transform Translation(float x, float y);
	static Transform Scale(float scale_x, float scale_y, float x, float y);
	static Transform Scale(float scale_x, float scale_y) { return Scale(scale_x, scale_y, 0.0f, 0.0f); }
	static Transform Rotation(float angle, float x, float y);
	static Transform Rotation(float angle) { return Rotation(angle, 0.0f, 0.0f); }
	static Transform Skew(float angle_x, float angle_y, float x, float y);
	static Transform Skew(float angle_x, float angle_y) { return Skew(angle_x, angle_y, 0.0f, 0.0f); }

	Transform operator*(const Transform& transform) const;
};


END_NAMESPACE(WndDesign)