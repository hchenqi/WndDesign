#pragma once

#include "WndObject.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


struct Relative {};
struct Assigned : Relative {};
struct Auto : Relative {};

template<class T> constexpr bool IsRelative = std::is_same_v<T, Relative>;
template<class T> constexpr bool IsAssigned = std::is_same_v<T, Assigned>;
template<class T> constexpr bool IsAuto = std::is_same_v<T, Auto>;

template<class WidthType, class HeightType>
struct LayoutType {
	static_assert(std::is_base_of_v<Relative, WidthType> && std::is_base_of_v<Relative, HeightType>, "invalid layout type");

	using width_type = WidthType;
	using height_type = HeightType;
};

template<class WidthType, class HeightType>
class WndType : public WndObject, public LayoutType<WidthType, HeightType> {};


template<class WidthType = Relative, class HeightType = Relative>
class child_ptr;

template<>
class child_ptr<Relative, Relative> : public std::unique_ptr<WndObject> {
public:
	child_ptr() {}
	template<class ChildType>
	child_ptr(std::unique_ptr<ChildType> ptr) : std::unique_ptr<WndObject>(ptr.release()) {}
	template<class ChildType>
	child_ptr(alloc_ptr<ChildType> ptr) : child_ptr(std::unique_ptr<ChildType>(ptr)) {}
public:
	operator WndObject& () const { return **this; }
	operator ref_ptr<WndObject> () const { return get(); }
};

template<class WidthType, class HeightType>
class child_ptr : public child_ptr<> {
public:
	child_ptr() {}
	template<class ChildType> requires (std::is_base_of_v<WidthType, typename ChildType::width_type> && std::is_base_of_v<HeightType, typename ChildType::height_type>)
	child_ptr(std::unique_ptr<ChildType> ptr) : child_ptr<>(std::move(ptr)) {}
	template<class ChildType> requires (std::is_base_of_v<WidthType, typename ChildType::width_type> && std::is_base_of_v<HeightType, typename ChildType::height_type>)
	child_ptr(alloc_ptr<ChildType> ptr) : child_ptr(std::unique_ptr<ChildType>(ptr)) {}
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
	using width_type = typename ChildType::width_type;
	using height_type = typename ChildType::height_type;
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