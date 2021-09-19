#pragma once

#include "style.h"
#include "../geometry/geometry_helper.h"


BEGIN_NAMESPACE(WndDesign)


struct StyleHelper : Style {
public:
	static const Size CalculateSize(ValueTag width, ValueTag height, Size size_ref) {
		width.ConvertToPixel(size_ref.width); height.ConvertToPixel(size_ref.height);
		return Size(width.AsUnsigned(), height.AsUnsigned());
	}
	static const std::pair<Size, Size> CalculateMinMaxSize(LengthStyle width, LengthStyle height, Size size_ref) {
		return { CalculateSize(width._min, height._min, size_ref), CalculateSize(width._max, height._max, size_ref) };
	}
private:
	static bool IsPositionAuto(ValueTag position) {
		return position.IsAuto() || position.IsCenter();
	}
	static const LengthStyle ConvertToPixel(LengthStyle length, uint length_ref) {
		length._normal.ConvertToPixel(length_ref);
		length._min.ConvertToPixel(length_ref);
		length._max.ConvertToPixel(length_ref);
		return length;
	}
	static uint CalculatePosition(ValueTag position_low, ValueTag position_high, uint length, uint length_ref) {
		if (position_low.IsAuto() || position_low.IsCenter()) {
			if (position_low.IsCenter()) {
				position_low = px((((int)length_ref) - (int)length) / 2);
			} else if (!position_high.IsAuto()) {
				position_low = px((int)length_ref - position_high.AsSigned() - (int)length);
			}
		}
		return position_low.AsSigned();
	}
	static const Interval CalculateLength(LengthStyle length, ValueTag position_low, ValueTag position_high, uint length_ref) {
		if (length_ref == 0) { return Interval(); }
		ValueTag& length_normal = length._normal.ConvertToPixel(length_ref);
		ValueTag& length_min = length._min.ConvertToPixel(length_ref);
		ValueTag& length_max = length._max.ConvertToPixel(length_ref);
		position_low.ConvertToPixel(length_ref);
		position_high.ConvertToPixel(length_ref);
		if (length_normal.IsAuto()) {
			if (IsPositionAuto(position_low) || IsPositionAuto(position_high)) {
				length_normal = length_max;
			} else {
				length_normal = px((int)length_ref - position_low.AsSigned() - position_high.AsSigned());
			}
		}
		return Interval(
			CalculatePosition(position_low, position_high, length_ref, length_normal.AsUnsigned()),
			Clamp(length_normal.AsUnsigned(), length_min.AsUnsigned(), length_max.AsUnsigned())
		);
	}
public:
	static const Rect CalculateRegion(LengthStyle width, LengthStyle height, PositionStyle position, Size size_ref) {
		return MakeRectFromInterval(
			CalculateLength(width, position._left, position._right, size_ref.width),
			CalculateLength(height, position._top, position._bottom, size_ref.height)
		);
	}
	static const Margin CalculateBorderMargin(BorderStyle border) {
		return Margin((int)border._width);
	}
	static const Margin CalculatePaddingMargin(PaddingStyle padding) {
		return Margin(padding._left, padding._top, padding._right, padding._bottom);
	}
};


END_NAMESPACE(WndDesign)