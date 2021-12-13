#pragma once

#include "WndFrame.h"
#include "../geometry/clamp.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ScrollFrame;


template<>
class ScrollFrame<Vertical> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
protected:
	Size size;
	uint child_height = 0;
	int frame_offset = 0;
public:
	Vector GetChildOffset() const { return Vector(0, -frame_offset); }
public:
	void UpdateFrameOffset(int offset) {
		frame_offset = child_height <= size.height ? 0 : clamp(offset, Interval(0, child_height - size.height));
		OnFrameOffsetUpdate(child_height, size.height, frame_offset);
		Redraw(region_infinite);
	}
	void Scroll(int offset) {
		if (offset == 0) { return; }
		UpdateFrameOffset(frame_offset + offset);
	}
	void ScrollIntoView(int y) {
		Scroll(y - clamp(y, Interval(frame_offset, size.height)));
	}
	void ScrollIntoView(Interval interval) {
		Scroll(interval.begin - clamp(interval, Interval(frame_offset, size.height)));
	}
private:
	virtual void OnFrameOffsetUpdate(uint content_height, uint frame_height, int frame_offset) {}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.width != size_ref.width) {
				child_height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (child_height != child_size.height) {
			child_height = child_size.height;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { Redraw(redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(-msg.wheel_delta); }
	}
};


template<>
class ScrollFrame<Horizontal> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size;
	uint child_width = 0;
	int frame_offset = 0;
private:
	Vector GetChildOffset() const { return Vector(-frame_offset, 0); }
public:
	void UpdateFrameOffset(int offset) {
		frame_offset = child_width <= size.width ? 0 : clamp(offset, Interval(0, child_width - size.width));
		OnFrameOffsetUpdate(child_width, size.width, frame_offset);
		Redraw(region_infinite);
	}
	int Scroll(int offset) {
		int assumed_frame_offset = frame_offset + offset;
		UpdateFrameOffset(assumed_frame_offset);
		return assumed_frame_offset - frame_offset;
	}
	void ScrollIntoView(int x) {
		Scroll(x - clamp(x, Interval(frame_offset, size.width)));
	}
	void ScrollIntoView(Interval interval) {
		Scroll(interval.begin - clamp(interval, Interval(frame_offset, size.width)));
	}
private:
	virtual void OnFrameOffsetUpdate(uint content_width, uint frame_width, int frame_offset) {}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.height != size_ref.height) {
				child_width = UpdateChildSizeRef(child, Size(length_max, size_ref.height)).width;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (child_width != child_size.width) {
			child_width = child_size.width;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { Redraw(redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll(-msg.wheel_delta); }
	}
};


template<>
class ScrollFrame<Bidirectional> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Auto>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size;
	Size child_size;
	Point frame_offset;
private:
	Vector GetChildOffset() const { return point_zero - frame_offset; }
public:
	void UpdateFrameOffset(Point offset) {
		frame_offset.x = child_size.width <= size.width ? 0 : clamp(offset.x, Interval(0, child_size.width - size.width));
		frame_offset.y = child_size.height <= size.height ? 0 : clamp(offset.y, Interval(0, child_size.height - size.height));
		OnFrameOffsetUpdate(child_size, size, frame_offset);
		Redraw(region_infinite);
	}
	Vector Scroll(Vector offset) {
		Point assumed_frame_offset = frame_offset + offset;
		UpdateFrameOffset(assumed_frame_offset);
		return assumed_frame_offset - frame_offset;
	}
	void ScrollIntoView(Point point) {
		Scroll(point - clamp(point, Rect(frame_offset, size)));
	}
	void ScrollIntoView(Rect rect) {
		Scroll(rect.point - clamp(rect, Rect(frame_offset, size)));
	}
private:
	virtual void OnFrameOffsetUpdate(Size content_size, Size frame_size, Point frame_offset) {}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			size = size_ref;
			child_size = UpdateChildSizeRef(child, Size(length_max, length_max));
			UpdateFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size != child_size) {
			this->child_size = child_size;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { Redraw(redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(Vector(-msg.wheel_delta, 0)); }
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll(Vector(0, -msg.wheel_delta)); }
	}
};


template<class WidthType, class HeightType>
struct deduce_scroll_frame_direction;

template<>
struct deduce_scroll_frame_direction<Assigned, Auto> {
	using type = Vertical;
};

template<>
struct deduce_scroll_frame_direction<Auto, Assigned> {
	using type = Horizontal;
};

template<>
struct deduce_scroll_frame_direction<Auto, Auto> {
	using type = Bidirectional;
};


template<class T>
ScrollFrame(T)->ScrollFrame<typename deduce_scroll_frame_direction<extract_width_type<T>, extract_height_type<T>>::type>;


END_NAMESPACE(WndDesign)