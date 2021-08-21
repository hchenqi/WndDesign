#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Vertical {};
struct Horizontal {};


struct Auto {};
struct Fixed {};

template<class WidthType, class HeightType>
class WndType : public WndObject {};

template<class WidthType, class HeightType>
class child_ptr : public std::unique_ptr<WndType<WidthType, HeightType>> {
	void VerifyNonNull() const { if (*this == nullptr) { throw std::invalid_argument("invalid child pointer"); } }
public:
	child_ptr(std::unique_ptr<WndObject> ptr) : std::unique_ptr<WndObject>(std::move(ptr)) { VerifyNonNull(); }
	operator WndObject& () const { VerifyNonNull(); return **this; }
};


END_NAMESPACE(WndDesign)