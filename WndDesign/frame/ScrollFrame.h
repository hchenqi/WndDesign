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
	float child_height = 0.0f;
	float frame_offset = 0.0f;
public:
	Vector GetChildOffset() const { return Vector(0.0f, -frame_offset); }
public:
	void UpdateFrameOffset(float offset) {
		frame_offset = child_height <= size.height ? 0.0f : clamp(offset, Interval(0.0f, child_height - size.height));
		OnFrameOffsetUpdate(child_height, size.height, frame_offset);
		redraw_region = region_infinite; Redraw();
	}
	void Scroll(float offset) {
		if (offset == 0.0f) { return; }
		UpdateFrameOffset(frame_offset + offset);
	}
	void ScrollIntoView(float y) {
		Scroll(y - clamp(y, Interval(frame_offset, size.height)));
	}
	void ScrollIntoView(Interval interval) {
		Scroll(interval.begin - clamp(interval, Interval(frame_offset, size.height)));
	}
private:
	virtual void OnFrameOffsetUpdate(float content_height, float frame_height, float frame_offset) {}
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.width != size_ref.width) {
				child_height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (child_height != child_size.height) {
			child_height = child_size.height;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
	virtual Transform GetChildTransform(WndObject& child) override { return GetChildOffset(); }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		redraw_region = child_redraw_region + GetChildOffset(); Redraw();
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll((float)-msg.wheel_delta); }
	}
};


template<>
class ScrollFrame<Horizontal> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	float child_width = 0.0f;
	float frame_offset = 0.0f;
private:
	Vector GetChildOffset() const { return Vector(-frame_offset, 0.0f); }
public:
	void UpdateFrameOffset(float offset) {
		frame_offset = child_width <= size.width ? 0.0f : clamp(offset, Interval(0.0f, child_width - size.width));
		OnFrameOffsetUpdate(child_width, size.width, frame_offset);
		redraw_region = region_infinite; Redraw();
	}
	void Scroll(float offset) {
		if (offset == 0.0f) { return; }
		UpdateFrameOffset(frame_offset + offset);
	}
	void ScrollIntoView(float x) {
		Scroll(x - clamp(x, Interval(frame_offset, size.width)));
	}
	void ScrollIntoView(Interval interval) {
		Scroll(interval.begin - clamp(interval, Interval(frame_offset, size.width)));
	}
private:
	virtual void OnFrameOffsetUpdate(float content_width, float frame_width, float frame_offset) {}
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.height != size_ref.height) {
				child_width = UpdateChildSizeRef(child, Size(length_max, size_ref.height)).width;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (child_width != child_size.width) {
			child_width = child_size.width;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
	virtual Transform GetChildTransform(WndObject& child) override { return GetChildOffset(); }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		redraw_region = child_redraw_region + GetChildOffset(); Redraw();
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll((float)-msg.wheel_delta); }
	}
};


template<>
class ScrollFrame<Bidirectional> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Auto>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size child_size;
	Point frame_offset;
private:
	Vector GetChildOffset() const { return point_zero - frame_offset; }
public:
	void UpdateFrameOffset(Point offset) {
		frame_offset.x = child_size.width <= size.width ? 0.0f : clamp(offset.x, Interval(0.0f, child_size.width - size.width));
		frame_offset.y = child_size.height <= size.height ? 0.0f : clamp(offset.y, Interval(0.0f, child_size.height - size.height));
		OnFrameOffsetUpdate(child_size, size, frame_offset);
		redraw_region = region_infinite; Redraw();
	}
	void Scroll(Vector offset) {
		if (offset == vector_zero) { return; }
		UpdateFrameOffset(frame_offset + offset);
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
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			size = size_ref;
			child_size = UpdateChildSizeRef(child, Size(length_max, length_max));
			UpdateFrameOffset(frame_offset);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size != child_size) {
			this->child_size = child_size;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
	virtual Transform GetChildTransform(WndObject& child) override { return GetChildOffset(); }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		redraw_region = child_redraw_region + GetChildOffset(); Redraw();
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(Vector((float)-msg.wheel_delta, 0.0f)); }
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll(Vector(0.0f, (float)-msg.wheel_delta)); }
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