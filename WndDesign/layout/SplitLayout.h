#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


struct First {};
struct Second {};


template<class Direction, class AutoChild>
class SplitLayout;


template<>
class SplitLayout<Vertical, First> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Assigned, Auto>;
	using child_ptr_second = child_ptr<Assigned, Assigned>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second) : first(std::move(first)), second(std::move(second)) {
		RegisterChild(this->first);
		RegisterChild(this->second);
	}

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
protected:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, length_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, length_first), Size(size.width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	void UpdateLayout() {
		length_second = max(0.0f, size.height - length_first);
		UpdateChildSizeRef(second, GetRegionSecond().size);
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size = size_ref;
		length_first = UpdateChildSizeRef(first, Size(size.width, length_min)).height;
		UpdateLayout();
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == first.get() && length_first != child_size.height) {
			length_first = child_size.height;
			UpdateLayout(); Redraw(GetRegionSecond());
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (point.y < length_first) { return first; }
		point.y -= length_first;
		if (point.y < length_second) { return second; }
		return nullptr;
	}
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
		DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
	}
};


template<>
class SplitLayout<Horizontal, Second> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Assigned, Assigned>;
	using child_ptr_second = child_ptr<Auto, Assigned>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second) : first(std::move(first)), second(std::move(second)) {
		RegisterChild(this->first);
		RegisterChild(this->second);
	}

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
private:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	void UpdateLayout() {
		length_first = max(0.0f, size.width - length_second);
		UpdateChildSizeRef(first, GetRegionFirst().size);
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size = size_ref;
		length_second = UpdateChildSizeRef(second, Size(length_min, size.height)).width;
		UpdateLayout();
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == second.get() && length_second != child_size.width) {
			length_second = child_size.width;
			UpdateLayout(); Redraw(GetRegionFirst());
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (point.x < length_first) { return first; }
		point.x -= length_first;
		if (point.x < length_second) { return second; }
		return nullptr;
	}
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
		DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
	}
};


END_NAMESPACE(WndDesign)