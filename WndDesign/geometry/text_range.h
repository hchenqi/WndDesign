#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct TextRange {
	size_t begin;
	size_t length;

	explicit constexpr TextRange() : begin(), length() {}
	explicit constexpr TextRange(size_t begin, size_t length) : begin(begin), length(length) {}

	constexpr bool operator==(const TextRange& range) const { return begin == range.begin && length == range.length; }
	constexpr bool operator!=(const TextRange& range) const { return begin != range.begin || length != range.length; }

	constexpr size_t left() const { return begin; }
	constexpr size_t right() const { return begin + length; }

	constexpr bool IsEmpty() const { return length == 0.0f; }

	constexpr bool Contains(size_t position) const { return position >= left() && position < right(); }
	constexpr bool Contains(const TextRange& range) const { return range.left() >= left() && range.right() <= right(); }

	constexpr TextRange Intersect(const TextRange& range) const {
		size_t left_max = max(left(), range.left()), right_min = min(right(), range.right());
		return right_min > left_max ? TextRange(left_max, right_min - left_max) : TextRange();
	}

	constexpr TextRange Union(const TextRange& range) const {
		size_t left_min = min(left(), range.left()), right_max = max(right(), range.right());
		return TextRange(left_min, right_max - left_min);
	}
};


constexpr TextRange text_range_empty;


END_NAMESPACE(WndDesign)