#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)


class ValuePixel {
private:
	int value;

public:
	explicit constexpr ValuePixel(int value) : value(value) {}

	constexpr ValuePixel operator-() const { return ValuePixel(-value); }

	constexpr int AsSigned() const { return value; }
	constexpr uint AsUnsigned() const { if (value < 0) { throw std::invalid_argument("invalid unsigned value"); } return value; }
};


class ValueTag {
public:
	enum class Tag { Pixel, Percent, Center, Auto, _Number };
	static_assert(static_cast<uint>(Tag::_Number) <= 0b1000);

private:
	uint value_tag;

private:
	constexpr int value() const { return static_cast<int>(value_tag) >> 3; }
	constexpr Tag tag() const { return static_cast<Tag>(value_tag & 0b111); }

public:
	explicit constexpr ValueTag(uint value, Tag tag) : value_tag((value << 3) | static_cast<uint>(tag)) {}
	explicit constexpr ValueTag(int value, Tag tag) : ValueTag(static_cast<uint>(value), tag) {}
	constexpr ValueTag(ValuePixel value) : ValueTag(value.AsSigned(), Tag::Pixel) {}

	constexpr bool IsPixel() const { return tag() == Tag::Pixel; }
	constexpr bool IsPercent() const { return tag() == Tag::Percent; }
	constexpr bool IsCenter() const { return tag() == Tag::Center; }
	constexpr bool IsAuto() const { return tag() == Tag::Auto; }

	constexpr ValueTag operator-() const { return ValueTag(-value(), tag()); }

	constexpr int AsSigned() const { return value(); }
	constexpr uint AsUnsigned() const { if (value() < 0) { throw std::invalid_argument("invalid unsigned value"); } return value(); }

	constexpr ValueTag& ConvertToPixel(uint entire_length) {
		if (tag() == Tag::Percent) { *this = ValueTag(value() * static_cast<int>(entire_length) / 100, Tag::Pixel); }
		return *this;
	}
};


#pragma warning (push)
#pragma warning (disable : 4455)   // literal suffix identifiers that do not start with an underscore are reserved.
constexpr ValuePixel operator""px(unsigned long long number) {
	return ValuePixel(static_cast<int>(number));
}
constexpr ValueTag operator""pct(unsigned long long number) {
	return ValueTag(static_cast<int>(number), ValueTag::Tag::Percent);
}
#pragma warning (pop)


constexpr ValuePixel px(int number) {
	return ValuePixel(number);
}
constexpr ValuePixel px(uint number) {
	return ValuePixel(static_cast<int>(number));
}
constexpr ValueTag pct(int number) {
	return ValueTag(number, ValueTag::Tag::Percent);
}
constexpr ValueTag pct(uint number) {
	return ValueTag(number, ValueTag::Tag::Percent);
}


constexpr ValueTag length_min_tag = ValueTag(length_min, ValueTag::Tag::Pixel);
constexpr ValueTag length_max_tag = ValueTag(length_max, ValueTag::Tag::Pixel);
constexpr ValueTag length_auto = ValueTag(0, ValueTag::Tag::Auto);

constexpr ValueTag position_min_tag = ValueTag(position_min, ValueTag::Tag::Pixel);
constexpr ValueTag position_max_tag = ValueTag(position_max, ValueTag::Tag::Pixel);
constexpr ValueTag position_center = ValueTag(0, ValueTag::Tag::Center);
constexpr ValueTag position_auto = length_auto;


END_NAMESPACE(WndDesign)