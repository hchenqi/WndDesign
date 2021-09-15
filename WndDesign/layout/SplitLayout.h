#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class SplitLayout;


template<>
class SplitLayout<Vertical, Auto> : public WndType<Relative, Auto> {
public:
	using child_ptr = child_ptr<Relative, Auto>;
public:
	SplitLayout(child_ptr first, child_ptr second) : first(std::move(first)), second(std::move(second)) {
		RegisterChild(this->first);
		RegisterChild(this->second);
	}
protected:
	child_ptr first;
	child_ptr second;
private:
	uint width;
	uint height_first;
	uint height_second;
private:
	const Size GetSize() const { return Size(width, height_first + height_second); }
	const Rect GetRegionFirst() const { return Rect(point_zero, Size(width, height_first)); }
	const Rect GetRegionSecond() const { return Rect(Point(0, (int)height_first), Size(width, height_second)); }
	const Rect GetChildRegion(const WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		if (width != size_ref.width) {
			width = size_ref.width;
			height_first = UpdateChildSizeRef(first, Size(width, length_min)).height;
			height_second = UpdateChildSizeRef(second, Size(width, length_min)).height;
		}
		return GetSize();
	}
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {
		if (&child == first.get()) {
			if (height_first != child_size.height) {
				height_first = child_size.height;
				SizeUpdated(GetSize());
			}
		} else {
			if (height_second != child_size.height) {
				height_second = child_size.height;
				SizeUpdated(GetSize());
			}
		}
	}
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override {
		Rect child_region = GetChildRegion(child);
		redraw_region = child_region.Intersect(redraw_region + (child_region.point - point_zero));
		if (!redraw_region.IsEmpty()) { Redraw(redraw_region); }
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {
		using pair = std::pair<WndObject&, Rect>;
		for (auto [child, child_region] : { pair{first, GetRegionFirst()}, pair{second, GetRegionSecond()} }) {
			Rect draw_region_child = child_region.Intersect(draw_region);
			if (!draw_region_child.IsEmpty()) { DrawChild(first, child_region.point, figure_queue, draw_region_child); }
		}
	}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.y < height_first) { return first; }
		point.y -= height_first;
		if ((uint)point.y < height_second) { return second; }
		return nullptr; 
	}
};


END_NAMESPACE(WndDesign)