#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Assigned {};
struct Relative {};
struct Auto {};

template<class WidthType, class HeightType>
struct layout_type {
	template<class T, class... Ts>
	static constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

	template<class T>
	static constexpr bool is_layout_type_v = is_any_of_v<T, Assigned, Relative, Auto>;

	static_assert(is_layout_type_v<WidthType> && is_layout_type_v<HeightType>, "invalid layout type");

	using width_type = WidthType;
	using height_type = HeightType;
};

template<class WidthType, class HeightType>
struct LayoutType {
	using layout_type = layout_type<WidthType, HeightType>;
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
	child_ptr() {}
	child_ptr(std::unique_ptr<WndObject> ptr) : std::unique_ptr<WndObject>(std::move(ptr)) { VerifyNonNull(); }
	child_ptr(alloc_ptr<WndObject> ptr) : child_ptr(std::unique_ptr<WndObject>(ptr)) {}
	operator WndObject& () const { VerifyNonNull(); return **this; }
	operator ref_ptr<WndObject> () const { VerifyNonNull(); return get(); }
};

template<class WidthType, class HeightType>
class child_ptr : public child_ptr<> {
public:
	child_ptr() {}
	template<class ChildType, class = std::enable_if_t<std::is_same_v<layout_type<WidthType, HeightType>, typename ChildType::layout_type>>>
	child_ptr(std::unique_ptr<ChildType> ptr) : child_ptr<>(std::move(ptr)) {}
	template<class ChildType, class = std::enable_if_t<std::is_same_v<layout_type<WidthType, HeightType>, typename ChildType::layout_type>>>
	child_ptr(alloc_ptr<ChildType> ptr) : child_ptr<>(ptr) {}
};


template<class T>
struct extract_layout_type;

template<class WidthType, class HeightType>
struct extract_layout_type<child_ptr<WidthType, HeightType>> {
	using width_type = WidthType;
	using height_type = HeightType;
};

template<class ChildType>
struct extract_layout_type<alloc_ptr<ChildType>> {
	using width_type = typename ChildType::layout_type::width_type;
	using height_type = typename ChildType::layout_type::height_type;
};

template<class ChildType>
struct extract_layout_type<std::unique_ptr<ChildType>> : public extract_layout_type<alloc_ptr<ChildType>>{};

template<class T>
using extract_width_type = typename extract_layout_type<T>::width_type;

template<class T>
using extract_height_type = typename extract_layout_type<T>::height_type;


struct Vertical {};
struct Horizontal {};
struct Bidirectional {};


END_NAMESPACE(WndDesign)