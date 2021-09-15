#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Relative {};  // dependent to size_ref provided by parent window (Assigned/Max/Relative)
struct Auto {};      // independent of parent window (Auto/Fixed)

template<class WidthType, class HeightType>
struct LayoutType {
	static_assert((std::is_same_v<WidthType, Relative> || std::is_same_v<WidthType, Auto>) &&
				  (std::is_same_v<HeightType, Relative> || std::is_same_v<HeightType, Auto>), "invalid size type");
};

template<class WidthType, class HeightType>
class WndType : public WndObject, public LayoutType<WidthType, HeightType> {};


template<class WidthType = void, class HeightType = void>
class child_ptr;

template<>
class child_ptr<void, void> : public std::unique_ptr<WndObject> {
private:
	void VerifyNonNull() const { if (*this == nullptr) { throw std::invalid_argument("invalid child pointer"); } }
public:
	child_ptr(std::unique_ptr<WndObject>&& ptr) : std::unique_ptr<WndObject>(std::move(ptr)) { VerifyNonNull(); }
	operator WndObject& () const { VerifyNonNull(); return **this; }
	operator ref_ptr<WndObject> () const { VerifyNonNull(); return get(); }
};

template<class WidthType, class HeightType>
class child_ptr : public child_ptr<> {
public:
	template<class ChildType, class = std::enable_if_t<std::is_base_of_v<LayoutType<WidthType, HeightType>, ChildType> && std::is_base_of_v<WndObject, ChildType>>>
	child_ptr(std::unique_ptr<ChildType>&& ptr) : child_ptr<>(std::move(ptr)) {}
};


struct Vertical {};
struct Horizontal {};


END_NAMESPACE(WndDesign)