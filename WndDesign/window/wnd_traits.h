#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Assigned {};
struct Relative {};
struct Auto {};

template<class WidthType, class HeightType>
class LayoutType {
	template <class T, class... Ts>
	static constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

	template <class T>
	static constexpr bool is_size_type_v = is_any_of_v<T, Assigned, Relative, Auto>;

	static_assert(is_size_type_v<WidthType> && is_size_type_v<HeightType>, "invalid size type");
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
	child_ptr(std::unique_ptr<WndObject> ptr) : std::unique_ptr<WndObject>(std::move(ptr)) { VerifyNonNull(); }
	child_ptr(alloc_ptr<WndObject> ptr) : child_ptr(std::unique_ptr<WndObject>(ptr)) {}
	operator WndObject& () const { VerifyNonNull(); return **this; }
	operator ref_ptr<WndObject> () const { VerifyNonNull(); return get(); }
};

template<class WidthType, class HeightType>
class child_ptr : public child_ptr<> {
public:
	template<class ChildType, class = std::enable_if_t<std::is_base_of_v<LayoutType<WidthType, HeightType>, ChildType> && std::is_base_of_v<WndObject, ChildType>>>
	child_ptr(std::unique_ptr<ChildType> ptr) : child_ptr<>(std::move(ptr)) {}
	template<class ChildType, class = std::enable_if_t<std::is_base_of_v<LayoutType<WidthType, HeightType>, ChildType> && std::is_base_of_v<WndObject, ChildType>>>
	child_ptr(alloc_ptr<ChildType> ptr) : child_ptr<>(ptr) {}
};


struct Vertical {};
struct Horizontal {};
struct Bidirectional {};


END_NAMESPACE(WndDesign)