#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Relative {};  // dependent to size_ref provided by parent window (Assigned/Max/Relative)
struct Auto {};      // independent of parent window (Auto/Fixed)

template<class WidthType, class HeightType>
class WndType : public WndObject {
	static_assert((std::is_same_v<WidthType, Relative> || std::is_same_v<WidthType, Auto>) &&
				  (std::is_same_v<HeightType, Relative> || std::is_same_v<HeightType, Auto>), "invalid size type");
};

template<class WidthType, class HeightType>
class child_ptr : public std::unique_ptr<WndObject> {
	void VerifyNonNull() const { if (*this == nullptr) { throw std::invalid_argument("invalid child pointer"); } }
public:
	child_ptr(std::unique_ptr<WndType<WidthType, HeightType>> ptr) : std::unique_ptr<WndObject>(std::move(ptr)) { VerifyNonNull(); }
	operator WndObject& () const { VerifyNonNull(); return **this; }
};


struct Vertical {};
struct Horizontal {};


END_NAMESPACE(WndDesign)