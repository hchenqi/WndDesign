#pragma once

#include "WndFrame.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class BorderFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	BorderFrame(ushort border_width, ushort border_radius, Color border_color, child_ptr child) :
		WndFrame(std::move(child)), border_width(border_width), border_radius(border_radius), border_color(border_color) {
	}
	BorderFrame(ushort border_width, Color border_color, child_ptr child) :
		BorderFrame(border_width, 0, border_color, std::move(child)) {
	}
protected:
	ushort border_width;
	ushort border_radius;
	Color border_color;
protected:
	Size size;
	Size child_size;
protected:
	Vector GetChildOffset() const { return Vector(border_width, border_width); }
	Rect GetChildRegion() const { return Rect(Point(border_width, border_width), child_size); }
protected:
	static Size ExtendSize(Size size, int length) {
		int width = (int)size.width + length * 2, height = (int)size.height + length * 2;
		return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
	}
	static Rect ExtendRegion(Rect region, int length) {
		return Rect(region.point - Vector(length, length), ExtendSize(region.size, length));
	}
	static uint square(int x) { return (uint)(x * x); };
	static bool PointInRoundedRectangle(Point point, Rect rect, uint radius) {
		if (!rect.Contains(point)) { return false; } if (radius == 0) { return true; }
		int x1 = rect.left() + (int)radius, x2 = rect.right() - (int)radius;
		int	y1 = rect.top() + (int)radius, y2 = rect.bottom() - (int)radius;
		int x = point.x, y = point.y;
		if (x < x1 && y < y1) { return square(x - x1) + square(y - y1) <= square(radius); }
		if (x < x1 && y > y2) { return square(x - x1) + square(y - y2) <= square(radius); }
		if (x > x2 && y < y1) { return square(x - x2) + square(y - y1) <= square(radius); }
		if (x > x2 && y > y2) { return square(x - x2) + square(y - y2) <= square(radius); }
		return true;
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		child_size = UpdateChildSizeRef(child, ExtendSize(size_ref, -(int)border_width));
		size = ExtendSize(child_size, (int)border_width); return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		size = ExtendSize(child_size, (int)border_width); SizeUpdated(size);
	}
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (PointInRoundedRectangle(point, GetChildRegion(), border_radius)) { point -= GetChildOffset(); return child; }
		return this;
	}
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		Redraw(redraw_region + GetChildOffset());
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
		if (border_width > 0 && border_color.IsVisible()) {
			border_radius > 0 ?
				figure_queue.add(point_zero, new RoundedRectangle(size, border_radius, border_width, border_color)) :
				figure_queue.add(point_zero, new Rectangle(size, border_width, border_color));
		}
	}
};


END_NAMESPACE(WndDesign)