#include "transform.h"

#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


Transform Transform::Identity() {
	return AsTransform(D2D1::Matrix3x2F::Identity());
}

Transform Transform::Translation(float x, float y) {
	return AsTransform(D2D1::Matrix3x2F::Translation(x, y));
}

Transform Transform::Scale(float scale_x, float scale_y, float x, float y) {
	return AsTransform(D2D1::Matrix3x2F::Scale(scale_x, scale_y, D2D1::Point2F(x, y)));
}

Transform Transform::Rotation(float angle, float x, float y) {
	return AsTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(x, y)));
}

Transform Transform::Skew(float angle_x, float angle_y, float x, float y) {
	return AsTransform(D2D1::Matrix3x2F::Skew(angle_x, angle_y, D2D1::Point2F(x, y)));
}


Transform Transform::Invert() const {
	Transform temp = *this; 
	if (!AsD2DTransform(temp).Invert()) { throw std::runtime_error("transform matrix is not invertible"); }
	return temp;
}


Transform operator*(const Transform& applied, const Transform& original) {
	return AsTransform(AsD2DTransform(applied) * AsD2DTransform(original));
}

Point operator*(Point point, const Transform& transform) {
	return AsPoint(AsD2DPoint(point) * AsD2DTransform(transform));
}


END_NAMESPACE(WndDesign)