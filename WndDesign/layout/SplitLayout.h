#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class _SplitLayout_Base : public WndObject {
protected:
	using child_type = child_ptr<>;

protected:
	_SplitLayout_Base(child_type child_first, child_type child_second) : child_first(std::move(child_first)), child_second(std::move(child_second)) {
		RegisterChild(this->child_first); RegisterChild(this->child_second);
	}

	// child
protected:
	child_type child_first;
	child_type child_second;
protected:
	bool IsFirst(WndObject& child) const { return &child == child_first.get(); }

	// layout
protected:
	Size size_ref;
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
};


class _SplitLayout_Base_Vertical : public _SplitLayout_Base {
protected:
	_SplitLayout_Base_Vertical(child_type child_first, child_type child_second) : _SplitLayout_Base(std::move(child_first), std::move(child_second)) {}

	// layout
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, length_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, length_first), Size(size.width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return IsFirst(child) ? GetRegionFirst() : GetRegionSecond(); }
protected:
	virtual Transform GetChildTransform(WndObject& child) const override {
		return GetChildRegion(child).point - point_zero;
	}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child_first, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(child_second, GetRegionSecond(), figure_queue, draw_region);
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (msg.point.y < length_first) { return child_first; }
		msg.point.y -= length_first;
		if (msg.point.y < length_second) { return child_second; }
		return nullptr;
	}
};

class _SplitLayout_Base_Horizontal : public _SplitLayout_Base {
protected:
	_SplitLayout_Base_Horizontal(child_type child_first, child_type child_second) : _SplitLayout_Base(std::move(child_first), std::move(child_second)) {}

	// layout
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return IsFirst(child) ? GetRegionFirst() : GetRegionSecond(); }
protected:
	virtual Transform GetChildTransform(WndObject& child) const override {
		return GetChildRegion(child).point - point_zero;
	}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child_first, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(child_second, GetRegionSecond(), figure_queue, draw_region);
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (msg.point.x < length_first) { return child_first; }
		msg.point.x -= length_first;
		if (msg.point.x < length_second) { return child_second; }
		return nullptr;
	}
};


template<class WidthTypeFirst, class HeightTypeFirst, class WidthTypeSecond, class HeightTypeSecond>
class SplitLayoutVertical : public _SplitLayout_Base_Vertical {
public:
	static_assert(IsAssigned<WidthTypeFirst> || IsAssigned<WidthTypeSecond>, "At least one child window's width type should be Assigned.");
	static_assert(IsAuto<HeightTypeFirst> || IsAuto<HeightTypeSecond>, "At least one child window's height type should be Auto.");
	static_assert((IsAssigned<WidthTypeFirst> || IsAuto<HeightTypeFirst>) && (IsAssigned<WidthTypeSecond> || IsAuto<HeightTypeSecond>), "Child window's height type must be Auto if its width type is not Assigned.");
public:
	using width_type = std::conditional_t<IsAssigned<WidthTypeFirst>, WidthTypeSecond, WidthTypeFirst>;
	using height_type = std::conditional_t<IsAuto<HeightTypeFirst>, HeightTypeSecond, HeightTypeFirst>;
public:
	using child_type_first = WndDesign::child_ptr<WidthTypeFirst, HeightTypeFirst>;
	using child_type_second = WndDesign::child_ptr<WidthTypeSecond, HeightTypeSecond>;

public:
	SplitLayoutVertical(child_type_first child_first, child_type_second child_second) : _SplitLayout_Base_Vertical(std::move(child_first), std::move(child_second)) {}

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		this->size_ref = size_ref;

		// update width
		if constexpr (IsAssigned<width_type>) {
			size.width = size_ref.width;
		}
		if constexpr (!IsAssigned<WidthTypeFirst> || (IsAssigned<width_type> && IsAuto<HeightTypeFirst>)) {
			Size child_size = UpdateChildSizeRef(child_first, size_ref);
			length_first = child_size.height;
			if constexpr (!IsAssigned<WidthTypeFirst>) {
				size.width = child_size.width;
			}
		}
		if constexpr (!IsAssigned<WidthTypeSecond> || (IsAssigned<width_type> && IsAuto<HeightTypeSecond>)) {
			Size child_size = UpdateChildSizeRef(child_second, size_ref);
			length_second = child_size.height;
			if constexpr (!IsAssigned<WidthTypeSecond>) {
				size.width = child_size.width;
			}
		}

		// update child size
		if constexpr (IsAssigned<WidthTypeFirst> && !(IsAssigned<width_type> && IsAuto<HeightTypeFirst>)) {
			Size child_size = UpdateChildSizeRef(child_first, Size(size.width, size_ref.height - length_second));
			length_first = child_size.height;
		}
		if constexpr (IsAssigned<WidthTypeSecond> && !(IsAssigned<width_type> && IsAuto<HeightTypeSecond>)) {
			Size child_size = UpdateChildSizeRef(child_second, Size(size.width, size_ref.height - length_first));
			length_second = child_size.height;
		}

		// update height
		size.height = length_first + length_second;

		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (IsFirst(child)) {
			length_first = child_size.height;
			if constexpr (!IsAssigned<WidthTypeFirst> || (IsAssigned<width_type> && IsAuto<HeightTypeFirst>)) {
				if constexpr (!IsAssigned<WidthTypeFirst>) {
					size.width = child_size.width;
				}
				Size child_size = UpdateChildSizeRef(child_second, Size(size.width, size_ref.height - length_first));
				length_second = child_size.height;
				Redraw(GetRegionSecond());
			}
		} else {
			length_second = child_size.height;
			if constexpr (!IsAssigned<WidthTypeSecond> || (IsAssigned<width_type> && IsAuto<HeightTypeSecond>)) {
				if constexpr (!IsAssigned<WidthTypeSecond>) {
					size.width = child_size.width;
				}
				Size child_size = UpdateChildSizeRef(child_first, Size(size.width, size_ref.height - length_second));
				length_first = child_size.height;
				Redraw(GetRegionFirst());
			}
		}
		size.height = length_first + length_second;
		SizeUpdated(size);
	}
};

template<class WidthTypeFirst, class HeightTypeFirst, class WidthTypeSecond, class HeightTypeSecond>
class SplitLayoutHorizontal : public _SplitLayout_Base_Horizontal {
public:
	static_assert(IsAssigned<HeightTypeFirst> || IsAssigned<HeightTypeSecond>, "At least one child window's height type should be Assigned.");
	static_assert(IsAuto<WidthTypeFirst> || IsAuto<WidthTypeSecond>, "At least one child window's width type should be Auto.");
	static_assert((IsAssigned<HeightTypeFirst> || IsAuto<WidthTypeFirst>) && (IsAssigned<HeightTypeSecond> || IsAuto<WidthTypeSecond>), "Child window's width type must be Auto if its height type is not Assigned.");
public:
	using width_type = std::conditional_t<IsAuto<WidthTypeFirst>, WidthTypeSecond, WidthTypeFirst>;
	using height_type = std::conditional_t<IsAssigned<HeightTypeFirst>, HeightTypeSecond, HeightTypeFirst>;
public:
	using child_type_first = WndDesign::child_ptr<WidthTypeFirst, HeightTypeFirst>;
	using child_type_second = WndDesign::child_ptr<WidthTypeSecond, HeightTypeSecond>;

public:
	SplitLayoutHorizontal(child_type_first child_first, child_type_second child_second) : _SplitLayout_Base_Horizontal(std::move(child_first), std::move(child_second)) {}

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		this->size_ref = size_ref;

		// update height
		if constexpr (IsAssigned<height_type>) {
			size.height = size_ref.height;
		}
		if constexpr (!IsAssigned<HeightTypeFirst> || (IsAssigned<height_type> && IsAuto<WidthTypeFirst>)) {
			Size child_size = UpdateChildSizeRef(child_first, size_ref);
			length_first = child_size.width;
			if constexpr (!IsAssigned<HeightTypeFirst>) {
				size.height = child_size.height;
			}
		}
		if constexpr (!IsAssigned<HeightTypeSecond> || (IsAssigned<height_type> && IsAuto<WidthTypeSecond>)) {
			Size child_size = UpdateChildSizeRef(child_second, size_ref);
			length_second = child_size.width;
			if constexpr (!IsAssigned<HeightTypeSecond>) {
				size.height = child_size.height;
			}
		}

		// update child size
		if constexpr (IsAssigned<HeightTypeFirst> && !(IsAssigned<height_type> && IsAuto<WidthTypeFirst>)) {
			Size child_size = UpdateChildSizeRef(child_first, Size(size_ref.width - length_second, size.height));
			length_first = child_size.width;
		}
		if constexpr (IsAssigned<HeightTypeSecond> && !(IsAssigned<height_type> && IsAuto<WidthTypeSecond>)) {
			Size child_size = UpdateChildSizeRef(child_second, Size(size_ref.width - length_first, size.height));
			length_second = child_size.width;
		}

		// update width
		size.width = length_first + length_second;

		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (IsFirst(child)) {
			length_first = child_size.width;
			if constexpr (!IsAssigned<HeightTypeFirst> || (IsAssigned<height_type> && IsAuto<WidthTypeFirst>)) {
				if constexpr (!IsAssigned<HeightTypeFirst>) {
					size.height = child_size.height;
				}
				Size child_size = UpdateChildSizeRef(child_second, Size(size_ref.width - length_first, size.height));
				length_second = child_size.width;
				Redraw(GetRegionSecond());
			}
		} else {
			length_second = child_size.width;
			if constexpr (!IsAssigned<HeightTypeSecond> || (IsAssigned<height_type> && IsAuto<WidthTypeSecond>)) {
				if constexpr (!IsAssigned<HeightTypeSecond>) {
					size.height = child_size.height;
				}
				Size child_size = UpdateChildSizeRef(child_first, Size(size_ref.width - length_second, size.height));
				length_first = child_size.width;
				Redraw(GetRegionFirst());
			}
		}
		size.width = length_first + length_second;
		SizeUpdated(size);
	}
};


template<class T1, class T2>
SplitLayoutVertical(T1, T2) -> SplitLayoutVertical<extract_width_type<T1>, extract_height_type<T1>, extract_width_type<T2>, extract_height_type<T2>>;

template<class T1, class T2>
SplitLayoutHorizontal(T1, T2) -> SplitLayoutHorizontal<extract_width_type<T1>, extract_height_type<T1>, extract_width_type<T2>, extract_height_type<T2>>;


END_NAMESPACE(WndDesign)