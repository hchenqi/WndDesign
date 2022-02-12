#include "transform.h"
#include "helper.h"

#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


Transform Transform::Identity() {
	return AsTransform(D2D1::Matrix3x2F::Identity());
}

Transform Transform::Translation(Vector offset) {
	return AsTransform(D2D1::Matrix3x2F::Translation(offset.x, offset.y));
}

Transform Transform::Scale(WndDesign::Scale scale, Point center) {
	return AsTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y, AsD2DPoint(center)));
}

Transform Transform::Rotation(float angle, Point center) {
	return AsTransform(D2D1::Matrix3x2F::Rotation(angle, AsD2DPoint(center)));
}

Transform Transform::Skew(float angle_x, float angle_y, Point center) {
	return AsTransform(D2D1::Matrix3x2F::Skew(angle_x, angle_y, AsD2DPoint(center)));
}

Transform Transform::Invert() const {
	D2D1::Matrix3x2F matrix = AsD2DTransform(*this);
	if (!matrix.Invert()) { throw std::runtime_error("transform matrix is not invertible"); }
	return AsTransform(matrix);
}

WndDesign::Scale Transform::GetScale() const {
	return WndDesign::Scale(sqrtf(square(matrix[0][0]) + square(matrix[0][1])), sqrtf(square(matrix[1][0]) + square(matrix[1][1])));
}

bool Transform::IsAxisAligned() const {
	return (matrix[0][0] == 0.0f || matrix[0][1] == 0.0f) && (matrix[1][0] == 0.0f || matrix[1][1] == 0.0f);
}


Transform operator*(const Transform& applied, const Transform& original) {
	return AsTransform(AsD2DTransform(applied) * AsD2DTransform(original));
}

Point operator*(Point point, const Transform& transform) {
	return AsPoint(AsD2DPoint(point) * AsD2DTransform(transform));
}


END_NAMESPACE(WndDesign)