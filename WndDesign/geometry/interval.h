#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Interval {
	int begin;
	uint length;

	explicit constexpr Interval() : begin(), length() {}
	explicit constexpr Interval(int begin, uint length) : begin(begin), length(length) {}
	explicit constexpr Interval(uint begin, uint length) : begin(static_cast<int>(begin)), length(length) {}

	constexpr bool operator==(const Interval& interval) const { return begin == interval.begin && length == interval.length; }
	constexpr bool operator!=(const Interval& interval) const { return begin != interval.begin || length != interval.length; }

	constexpr int left() const { return begin; }
	constexpr int right() const { return begin + (int)length; }

	constexpr bool IsEmpty() const { return length == 0; }

	constexpr bool Contains(int number) const { return number >= left() && number < right(); }
	constexpr bool Contains(const Interval& interval) const { return left() >= interval.left() && right() <= interval.right(); }

	constexpr Interval Intersect(const Interval& interval) const {
		int left_max = max(left(), interval.left()), right_min = min(right(), interval.right());
		return right_min > left_max ? Interval(left_max, (uint)(right_min - left_max)) : Interval();
	}

	constexpr Interval Union(const Interval& interval) const {
		if (IsEmpty()) { return interval; } if (interval.IsEmpty()) { return *this; }
		int left_min = min(left(), interval.left()), right_max = max(right(), interval.right());
		return Interval(left_min, (uint)(right_max - left_min));
	}
};


END_NAMESPACE(WndDesign)