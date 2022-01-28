#pragma once

#include "vector.h"
#include "point.h"


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

	Transform() = default;
	Transform(Vector vector) : Transform(Translation((float)vector.x, (float)vector.y)) {}

	Transform Invert() const;
};


Transform operator*(const Transform& applied, const Transform& original);

Point operator*(const Point& point, const Transform& transform);


END_NAMESPACE(WndDesign)