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
protected:
	child_ptr_first first;
	child_ptr_second second;
private:
	Size size;
	uint height_first = 0;
	uint height_second = 0;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, height_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0, (int)height_first), Size(size.width, height_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	void UpdateHeightSecond() {
		height_second = size.height > height_first ? size.height - height_first : 0;
		UpdateChildSizeRef(second, GetRegionSecond().size);
	}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size = size_ref;
		height_first = UpdateChildSizeRef(first, Size(size.width, length_min)).height;
		UpdateHeightSecond();
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == first.get() && height_first != child_size.height) {
			height_first = child_size.height;
			UpdateHeightSecond(); Redraw(GetRegionSecond());
		}
	}
private:
	virtual Vector GetChildOffset(WndObject& child) override {
		return GetChildRegion(child).point - point_zero;
	}
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.y < height_first) { return first; }
		point.y -= height_first;
		if ((uint)point.y < height_second) { return second; }
		return nullptr;
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		Rect child_region = GetChildRegion(child);
		redraw_region = child_region.Intersect(redraw_region + (child_region.point - point_zero));
		if (!redraw_region.IsEmpty()) { Redraw(redraw_region); }
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
protected:
	child_ptr_first first;
	child_ptr_second second;
private:
	Size size;
	uint width_first = 0;
	uint width_second = 0;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(width_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point((int)width_first, 0), Size(width_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	void UpdateWidthFirst() {
		width_first = size.width > width_second ? size.width - width_second : 0;
		UpdateChildSizeRef(first, GetRegionFirst().size);
	}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size = size_ref;
		width_second = UpdateChildSizeRef(second, Size(length_min, size.height)).width;
		UpdateWidthFirst();
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == second.get() && width_second != child_size.width) {
			width_second = child_size.width;
			UpdateWidthFirst(); Redraw(GetRegionFirst());
		}
	}
private:
	virtual Vector GetChildOffset(WndObject& child) override {
		return GetChildRegion(child).point - point_zero;
	}
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.x < width_first) { return first; }
		point.x -= width_first;
		if ((uint)point.x < width_second) { return second; }
		return nullptr;
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		Rect child_region = GetChildRegion(child);
		redraw_region = child_region.Intersect(redraw_region + (child_region.point - point_zero));
		if (!redraw_region.IsEmpty()) { Redraw(redraw_region); }
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
	}
};


END_NAMESPACE(WndDesign)