#pragma once

#include "style.h"
#include "../geometry/interval.h"


BEGIN_NAMESPACE(WndDesign)


struct StyleHelper {
public:
	static constexpr Size CalculateSize(ValueTag width, ValueTag height, Size size_ref) {
		return Size(width.ConvertToPixel(size_ref.width).AsUnsigned(), height.ConvertToPixel(size_ref.height).AsUnsigned());
	}
	static constexpr std::pair<Size, Size> CalculateMinMaxSize(LengthStyle width, LengthStyle height, Size size_ref) {
		return { CalculateSize(width._min, height._min, size_ref), CalculateSize(width._max, height._max, size_ref) };
	}
private:
	static constexpr uint clamp(uint length_normal, uint length_min, uint length_max) {
		if (length_normal < length_min) { length_normal = length_min; }
		if (length_normal > length_max) { length_normal = length_max; }
		return length_normal;
	}
	static constexpr Rect MakeRectFromInterval(Interval horizontal, Interval vertical) {
		return Rect(horizontal.begin, vertical.begin, horizontal.length, vertical.length);
	}
	static constexpr bool IsPositionAuto(ValueTag position) { return position.IsAuto() || position.IsCenter(); }
	static constexpr uint CalculatePosition(ValueTag position_low, ValueTag position_high, uint length, uint length_ref) {
		if (IsPositionAuto(position_low)) {
			if (position_low.IsCenter()) {
				position_low = px((((int)length_ref) - (int)length) / 2);
			} else if (!position_high.IsAuto()) {
				position_low = px((int)length_ref - position_high.AsSigned() - (int)length);
			}
		}
		return position_low.AsSigned();
	}
	static constexpr Interval CalculateLength(LengthStyle length, ValueTag position_low, ValueTag position_high, uint length_ref) {
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
			CalculatePosition(position_low, position_high, length_normal.AsUnsigned(), length_ref),
			clamp(length_normal.AsUnsigned(), length_min.AsUnsigned(), length_max.AsUnsigned())
		);
	}
public:
	static constexpr Rect CalculateRegion(LengthStyle width, LengthStyle height, PositionStyle position, Size size_ref) {
		return MakeRectFromInterval(
			CalculateLength(width, position._left, position._right, size_ref.width),
			CalculateLength(height, position._top, position._bottom, size_ref.height)
		);
	}
};


END_NAMESPACE(WndDesign)