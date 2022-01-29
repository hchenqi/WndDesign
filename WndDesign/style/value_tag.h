#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)


class ValueTag {
public:
	enum class Type { Pixel, Percent, Center, Auto, _Number };

private:
	Type _type;
	float _value;

public:
	explicit constexpr ValueTag(Type type, float value) : _type(type), _value(value) {}

	constexpr bool IsPixel() const { return _type == Type::Pixel; }
	constexpr bool IsPercent() const { return _type == Type::Percent; }
	constexpr bool IsCenter() const { return _type == Type::Center; }
	constexpr bool IsAuto() const { return _type == Type::Auto; }

	constexpr float value() const { return _value; }

	constexpr ValueTag operator-() const { return ValueTag(_type, -_value); }

	constexpr ValueTag& ConvertToPixel(float length_ref) {
		if (_type == Type::Percent) { _value = length_ref * _value / 100.0f; _type = Type::Pixel; } return *this;
	}
};


#pragma warning (push)
#pragma warning (disable : 4455)  // literal suffix identifiers that do not start with an underscore are reserved.
constexpr ValueTag operator""px(long double number) {
	return ValueTag(ValueTag::Type::Pixel, (float)number);
}

constexpr ValueTag operator""px(unsigned long long number) {
	return ValueTag(ValueTag::Type::Pixel, (float)number);
}

constexpr ValueTag operator""pct(long double number) {
	return ValueTag(ValueTag::Type::Percent, (float)number);
}

constexpr ValueTag operator""pct(unsigned long long number) {
	return ValueTag(ValueTag::Type::Percent, (float)number);
}
#pragma warning (pop)


constexpr ValueTag px(int number) {
	return ValueTag(ValueTag::Type::Pixel, (float)number);
}

constexpr ValueTag px(uint number) {
	return ValueTag(ValueTag::Type::Pixel, (float)number);
}

constexpr ValueTag px(float number) {
	return ValueTag(ValueTag::Type::Pixel, number);
}

constexpr ValueTag pct(int number) {
	return ValueTag(ValueTag::Type::Percent, (float)number);
}

constexpr ValueTag pct(uint number) {
	return ValueTag(ValueTag::Type::Percent, (float)number);
}

constexpr ValueTag pct(float number) {
	return ValueTag(ValueTag::Type::Percent, number);
}


constexpr ValueTag length_min_tag = ValueTag(ValueTag::Type::Pixel, length_min);
constexpr ValueTag length_max_tag = ValueTag(ValueTag::Type::Pixel, length_max);
constexpr ValueTag length_auto = ValueTag(ValueTag::Type::Auto, 0.0f);

constexpr ValueTag position_min_tag = ValueTag(ValueTag::Type::Pixel, position_min);
constexpr ValueTag position_max_tag = ValueTag(ValueTag::Type::Pixel, position_max);
constexpr ValueTag position_center = ValueTag(ValueTag::Type::Center, 0.0f);
constexpr ValueTag position_auto = length_auto;


END_NAMESPACE(WndDesign)