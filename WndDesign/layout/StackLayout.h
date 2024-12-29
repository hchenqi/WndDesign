#pragma once

#include "../window/wnd_traits.h"
#include "../common/reversion_wrapper.h"


BEGIN_NAMESPACE(WndDesign)


class _StackLayout_Base : public WndObject {
protected:
	_StackLayout_Base(child_ptr<> child_first, child_ptr<> child_second) : child_first(std::move(child_first)), child_second(std::move(child_second)) {
		RegisterChild(this->child_first); RegisterChild(this->child_second);
	}

	// child
protected:
	child_ptr<> child_first;
	child_ptr<> child_second;

	// layout
protected:
	Size size;
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return Transform::Identity(); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { Redraw(child_redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child_first, point_zero, figure_queue, draw_region);
		DrawChild(child_second, point_zero, figure_queue, draw_region);
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		MouseMsg msg_copy = msg;
		if (ref_ptr<WndObject> res = HitTestChild(child_second, msg)) { return res; }
		return HitTestChild(child_first, msg = msg_copy);
	}
};


template<class WidthTypeFirst, class HeightTypeFirst, class WidthTypeSecond, class HeightTypeSecond>
class StackLayout : public _StackLayout_Base {
public:
	static_assert((IsAssigned<WidthTypeFirst> && IsAssigned<HeightTypeFirst>) || (IsAssigned<WidthTypeSecond> && IsAssigned<HeightTypeSecond>), "At least one child window's width type and height type should be Assigned.");
public:
	using width_type = std::conditional_t<IsAssigned<WidthTypeFirst>, WidthTypeSecond, WidthTypeFirst>;
	using height_type = std::conditional_t<IsAssigned<HeightTypeFirst>, HeightTypeSecond, HeightTypeFirst>;
public:
	using child_type_first = child_ptr<WidthTypeFirst, HeightTypeFirst>;
	using child_type_second = child_ptr<WidthTypeSecond, HeightTypeSecond>;

public:
	StackLayout(child_type_first child_first, child_type_second child_second) : _StackLayout_Base(std::move(child_first), std::move(child_second)) {}

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size = size_ref;

		if constexpr (!IsAssigned<WidthTypeFirst> || !IsAssigned<HeightTypeFirst>) {
			size = UpdateChildSizeRef(child_first, size);
		}
		if constexpr (!IsAssigned<WidthTypeSecond> || !IsAssigned<HeightTypeSecond>) {
			size = UpdateChildSizeRef(child_second, size);
		}

		if constexpr (IsAssigned<WidthTypeFirst> && IsAssigned<HeightTypeFirst>) {
			UpdateChildSizeRef(child_first, size);
		}
		if constexpr (IsAssigned<WidthTypeSecond> && IsAssigned<HeightTypeSecond>) {
			UpdateChildSizeRef(child_second, size);
		}

		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == child_first.get()) {
			if constexpr (!IsAssigned<WidthTypeFirst> || !IsAssigned<HeightTypeFirst>) {
				size = child_size;
				UpdateChildSizeRef(child_second, size);
				SizeUpdated(size);
			}
		} else {
			if constexpr (!IsAssigned<WidthTypeSecond> || !IsAssigned<HeightTypeSecond>) {
				size = child_size;
				UpdateChildSizeRef(child_first, size);
				SizeUpdated(size);
			}
		}
	}
};


template<class T1, class T2>
StackLayout(T1, T2) -> StackLayout<extract_width_type<T1>, extract_height_type<T1>, extract_width_type<T2>, extract_height_type<T2>>;


class StackLayoutMultiple : public WndType<Assigned, Assigned> {
public:
	using child_type = child_ptr<Assigned, Assigned>;

public:
	template<class... Ts>
	StackLayoutMultiple(Ts... child_args) : child_list() {
		(RegisterChild(child_args), ...);
		child_list.reserve(sizeof...(Ts)); (child_list.emplace_back(std::move(child_args)), ...);
	}

	// child
protected:
	std::vector<child_type> child_list;

	// layout
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return Transform::Identity(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { for (auto& child : child_list) { UpdateChildSizeRef(child, size_ref); } return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { Redraw(child_redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { for (auto& child : child_list) { DrawChild(child, point_zero, figure_queue, draw_region); } }

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		MouseMsg msg_copy = msg;
		for (auto& child : reverse(child_list)) {
			if (ref_ptr<WndObject> res = HitTestChild(child, msg)) {
				return res;
			}
			msg = msg_copy;
		}
		return nullptr;
	}
};


END_NAMESPACE(WndDesign)